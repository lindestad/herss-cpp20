#ifndef HERSS_TEST_PATHS_H
#define HERSS_TEST_PATHS_H

#include <filesystem>
#include <string>

#ifndef HERSS_TEST_DATA_DIR
#define HERSS_TEST_DATA_DIR "../src_tests/utahps_test"
#endif

#ifndef HERSS_TEST_EXECUTABLE
#define HERSS_TEST_EXECUTABLE "./herss.exe"
#endif

inline std::string herssTestDataDir()
{
    return std::filesystem::path(HERSS_TEST_DATA_DIR).string();
}

inline std::string herssTestDataPath(const char* filename)
{
    return (std::filesystem::path(HERSS_TEST_DATA_DIR) / filename).string();
}

inline std::string herssTestOutputDir()
{
    return (std::filesystem::path(HERSS_TEST_DATA_DIR) / "output").string() + "/";
}

inline std::string herssTestExecutablePath()
{
    return std::filesystem::path(HERSS_TEST_EXECUTABLE).string();
}

#endif
