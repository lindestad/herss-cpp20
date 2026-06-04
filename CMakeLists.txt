cmake_minimum_required(VERSION 3.24)

project(herss
    VERSION 3.1.1
    DESCRIPTION "Hydraulic Economic River System Simulator"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(HERSS_ENABLE_NATIVE "Build with -march=native" OFF)

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

add_executable(herss_cli src/main.cpp)
target_link_libraries(herss_cli PRIVATE herss_core)
set_target_properties(herss_cli PROPERTIES OUTPUT_NAME herss.exe)

foreach(target IN ITEMS herss_core herss_shared herss_cli)
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
    target_include_directories(herss_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src_tests
    )
    target_compile_definitions(herss_tests PRIVATE
        HERSS_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/src_tests/utahps_test"
        HERSS_TEST_EXECUTABLE="$<TARGET_FILE:herss_cli>"
    )

    include(GoogleTest)
    gtest_discover_tests(herss_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DISCOVERY_TIMEOUT 60
    )
endif()
