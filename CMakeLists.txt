cmake_minimum_required(VERSION 3.28)

project(herss
    VERSION 3.1.1
    DESCRIPTION "Hydraulic Economic River System Simulator"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

option(HERSS_ENABLE_NATIVE "Build with -march=native" OFF)
option(HERSS_ENABLE_CPPYY_TEST "Run cppyy binding regression tests" ON)

set(HERSS_CORE_SOURCES
    src/node.cpp
    src/globalconfig.cpp
    src/line.cpp
    src/reservoir.cpp
    src/dataset.cpp
    src/qmin.cpp
    src/powerstation.cpp
    src/channel.cpp
    src/riversystem.cpp
    src/scenario.cpp
    src/herss.cpp
    src/arraycurve.cpp
    src/xtime.cpp
    src/topoparser.cpp
    src/logger.cpp
    src/cascadedreservoirs.cpp
)

add_library(herss_core STATIC ${HERSS_CORE_SOURCES})
target_include_directories(herss_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(herss_core PUBLIC cxx_std_20)
set_target_properties(herss_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(herss_shared SHARED ${HERSS_CORE_SOURCES})
target_include_directories(herss_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(herss_shared PUBLIC cxx_std_20)
set_target_properties(herss_shared PROPERTIES
    OUTPUT_NAME herss
    POSITION_INDEPENDENT_CODE ON
)

add_library(herss_module)
target_sources(herss_module
    PUBLIC
        FILE_SET CXX_MODULES
        FILES src/herss.cppm
)
target_link_libraries(herss_module PUBLIC herss_core)
target_include_directories(herss_module PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(herss_module PUBLIC cxx_std_20)

add_executable(herss_cli src/main.cpp)
target_link_libraries(herss_cli PRIVATE herss_core)
set_target_properties(herss_cli PROPERTIES OUTPUT_NAME herss)

foreach(target IN ITEMS herss_core herss_shared herss_module herss_cli)
    target_compile_options(${target} PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    )
    if(HERSS_ENABLE_NATIVE)
        target_compile_options(${target} PRIVATE
            $<$<CXX_COMPILER_ID:GNU,Clang>:-march=native>
        )
    endif()
endforeach()

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    set(HERSS_TEST_SOURCES
        src_tests/test_arraycurve.cpp
        src_tests/test_channel.cpp
        src_tests/test_dataset.cpp
        src_tests/test_globalconfig.cpp
        src_tests/test_golden_output.cpp
        src_tests/test_interop.cpp
        src_tests/test_line.cpp
        src_tests/test_powerstation.cpp
        src_tests/test_reservoir.cpp
        src_tests/test_riversystem.cpp
        src_tests/test_runtime.cpp
        src_tests/test_valuefunction.cpp
        src_tests/test_waterbalance.cpp
    )

    add_executable(herss_tests ${HERSS_TEST_SOURCES})
    target_link_libraries(herss_tests PRIVATE herss_core GTest::gtest_main)
    add_dependencies(herss_tests herss_cli)
    target_include_directories(herss_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src_tests
    )
    target_compile_definitions(herss_tests PRIVATE
        HERSS_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src_tests/utahps_test"
        HERSS_TEST_EXECUTABLE="$<TARGET_FILE:herss_cli>"
        HERSS_TEST_REFERENCE_OUTPUT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src_tests/utahps_test/reference_output"
    )

    include(GoogleTest)
    gtest_discover_tests(herss_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DISCOVERY_TIMEOUT 60
    )

    add_executable(herss_module_smoke src_tests/module_smoke.cpp)
    target_link_libraries(herss_module_smoke PRIVATE herss_module)
    add_test(NAME HerssModuleSmoke COMMAND herss_module_smoke)

    add_executable(herss_header_compat_smoke src_tests/header_compat_smoke.cpp)
    target_link_libraries(herss_header_compat_smoke PRIVATE herss_core)
    target_include_directories(herss_header_compat_smoke PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
    add_test(NAME HerssHeaderCompatSmoke COMMAND herss_header_compat_smoke)

    if(HERSS_ENABLE_CPPYY_TEST)
        find_program(HERSS_CPPYY_PYTHON
            NAMES python3.12 python3.13 python3
            DOC "Python interpreter with cppyy installed"
        )
        if(NOT HERSS_CPPYY_PYTHON)
            message(FATAL_ERROR "HERSS_ENABLE_CPPYY_TEST is ON, but no Python interpreter was found")
        endif()

        set(HERSS_CPPYY_EXTRA_CLING_ARGS "" CACHE STRING "Extra Cling arguments used when importing cppyy")

        if(NOT HERSS_CPPYY_EXTRA_CLING_ARGS)
            find_program(HERSS_CPPYY_GXX15 NAMES g++-15)
            if(HERSS_CPPYY_GXX15)
                execute_process(
                    COMMAND ${HERSS_CPPYY_GXX15} -E -x c++ - -v
                    INPUT_FILE /dev/null
                    ERROR_VARIABLE HERSS_CPPYY_GXX15_INCLUDE_TRACE
                    OUTPUT_QUIET
                )
                string(REGEX MATCH "#include <\\.\\.\\.> search starts here:\n([^\n]*\n)*End of search list\\."
                    HERSS_CPPYY_INCLUDE_BLOCK "${HERSS_CPPYY_GXX15_INCLUDE_TRACE}")
                string(REPLACE "#include <...> search starts here:\n" "" HERSS_CPPYY_INCLUDE_BLOCK "${HERSS_CPPYY_INCLUDE_BLOCK}")
                string(REPLACE "End of search list." "" HERSS_CPPYY_INCLUDE_BLOCK "${HERSS_CPPYY_INCLUDE_BLOCK}")
                string(REPLACE "\n" ";" HERSS_CPPYY_INCLUDE_DIRS "${HERSS_CPPYY_INCLUDE_BLOCK}")

                set(HERSS_CPPYY_EXTRA_CLING_ARGS "-nostdinc++")
                foreach(include_dir IN LISTS HERSS_CPPYY_INCLUDE_DIRS)
                    string(STRIP "${include_dir}" include_dir)
                    if(include_dir AND EXISTS "${include_dir}")
                        string(APPEND HERSS_CPPYY_EXTRA_CLING_ARGS " -isystem${include_dir}")
                    endif()
                endforeach()
            endif()
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} -E env
                STDCXX=20
                "EXTRA_CLING_ARGS=${HERSS_CPPYY_EXTRA_CLING_ARGS}"
                ${HERSS_CPPYY_PYTHON} -c "import cppyy"
            RESULT_VARIABLE HERSS_CPPYY_IMPORT_RESULT
            OUTPUT_QUIET
            ERROR_QUIET
        )
        if(NOT HERSS_CPPYY_IMPORT_RESULT EQUAL 0)
            message(FATAL_ERROR
                "HERSS_ENABLE_CPPYY_TEST is ON, but cppyy could not be imported by ${HERSS_CPPYY_PYTHON}. "
                "Install cppyy for that interpreter or set HERSS_CPPYY_PYTHON/HERSS_CPPYY_EXTRA_CLING_ARGS."
            )
        endif()

        add_test(NAME HerssCppyyInterop
            COMMAND ${HERSS_CPPYY_PYTHON}
                ${CMAKE_CURRENT_SOURCE_DIR}/src_tests/test_cppyy_interop.py
                --source-dir ${CMAKE_CURRENT_SOURCE_DIR}
                --library $<TARGET_FILE:herss_shared>
        )
        set_tests_properties(HerssCppyyInterop PROPERTIES
            ENVIRONMENT "STDCXX=20;EXTRA_CLING_ARGS=${HERSS_CPPYY_EXTRA_CLING_ARGS}"
        )
    endif()
endif()
