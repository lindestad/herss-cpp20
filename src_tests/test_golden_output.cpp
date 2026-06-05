#include <gtest/gtest.h>
#include "test_paths.h"

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

namespace {

std::string quotePath(const std::filesystem::path& path)
{
    std::string quoted = "'";
    for (char ch : path.string()) {
        if (ch == '\'') {
            quoted += "'\\''";
        } else {
            quoted += ch;
        }
    }
    quoted += "'";
    return quoted;
}

std::string normalizedFileContents(const std::filesystem::path& path)
{
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        ADD_FAILURE() << "Could not open " << path;
        return {};
    }

    std::string contents((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    contents.erase(std::remove(contents.begin(), contents.end(), '\r'), contents.end());
    return contents;
}

std::vector<std::filesystem::path> regularFilesIn(const std::filesystem::path& directory)
{
    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            files.push_back(entry.path().filename());
        }
    }
    std::sort(files.begin(), files.end());
    return files;
}

void writeGlobalFile(const std::filesystem::path& globalFile,
                     const std::filesystem::path& inputDir,
                     const std::filesystem::path& outputDir)
{
    std::ofstream output(globalFile);
    ASSERT_TRUE(output) << "Could not write " << globalFile;

    output << "# Golden-output regression fixture for uTAHPS\n"
           << "SYSTEMNAME uTAHPS\n"
           << "INPUTDIR " << inputDir.string() << "/\n"
           << "ACTIONFILE actions.txt\n"
           << "INFLOWFILE inflow.txt\n"
           << "PRICEFILE pricefile.txt\n"
           << "TOPOLOGYFILE topology.txt\n"
           << "STARTSTATEFILE start_state.txt\n"
           << "DT 3600\n"
           << "DT_LAST 3600\n"
           << "OUTPUTFILE output_utahps.txt\n"
           << "OUTSTATEFILE outstate_utahps.txt\n"
           << "WRITE_NODEFILES 1\n"
           << "OUTPUTDIR " << outputDir.string() << "/\n";
}

std::filesystem::path makeRunDirectory()
{
    const auto unique = std::chrono::steady_clock::now().time_since_epoch().count();
    return std::filesystem::temp_directory_path() / ("herss_utahps_golden_" + std::to_string(unique));
}

} // namespace

TEST(GoldenOutputTest, UtahpsExecutableOutputMatchesReferenceFiles)
{
    const std::filesystem::path executable = herssTestExecutablePath();
    const std::filesystem::path fixtureDir = herssTestDataDir();
    const std::filesystem::path referenceOutputDir = herssTestReferenceOutputDir();

    ASSERT_TRUE(std::filesystem::exists(executable)) << executable;
    ASSERT_TRUE(std::filesystem::exists(fixtureDir)) << fixtureDir;
    ASSERT_TRUE(std::filesystem::exists(referenceOutputDir)) << referenceOutputDir;

    const std::filesystem::path runDir = makeRunDirectory();
    const std::filesystem::path inputDir = runDir / "input";
    const std::filesystem::path outputDir = runDir / "output";

    std::filesystem::create_directories(inputDir);
    std::filesystem::create_directories(outputDir);

    const std::vector<std::string> inputFiles = {
        "actions.txt",
        "inflow.txt",
        "pricefile.txt",
        "start_state.txt",
        "topology.txt",
    };

    for (const auto& filename : inputFiles) {
        std::filesystem::copy_file(
            fixtureDir / filename,
            inputDir / filename,
            std::filesystem::copy_options::overwrite_existing);
    }

    const std::filesystem::path globalFile = runDir / "global.txt";
    writeGlobalFile(globalFile, inputDir, outputDir);

    const std::string command = "cd " + quotePath(runDir) + " && " + quotePath(executable) + " " + quotePath(globalFile);
    const int exitCode = std::system(command.c_str());
    ASSERT_EQ(exitCode, 0) << command;

    const auto expectedFiles = regularFilesIn(referenceOutputDir);
    const auto actualFiles = regularFilesIn(outputDir);
    ASSERT_EQ(actualFiles, expectedFiles);

    for (const auto& relativePath : expectedFiles) {
        const std::filesystem::path expectedPath = referenceOutputDir / relativePath;
        const std::filesystem::path actualPath = outputDir / relativePath;
        EXPECT_EQ(normalizedFileContents(actualPath), normalizedFileContents(expectedPath))
            << "Generated output differs for " << relativePath;
    }

    std::filesystem::remove_all(runDir);
}
