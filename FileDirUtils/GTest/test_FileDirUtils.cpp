// FileDirUtils_test.cpp
#include <filesystem>
#include <fstream>
#include "gtest/gtest.h"
#include "FileDirUtils.h"
#include "Logger.h"

namespace fs = std::filesystem;

TEST( FileDirUtilsTest, CreateDeleteDirectory )
{
    std::string testDir = "test_dir";

    // Create Directory
    EXPECT_TRUE( FileDirUtils::createDirectory( testDir ) );
    EXPECT_TRUE( FileDirUtils::directoryExists( testDir ) );

    // Delete Directory
    EXPECT_TRUE( FileDirUtils::deleteDirectory( testDir ) );
    EXPECT_FALSE( FileDirUtils::directoryExists( testDir ) );
}

TEST( FileDirUtilsTest, ListFiles )
{
    std::string testDir = "test_dir";
    std::string testFilePath = "test_dir/test_file.txt"; // Renamed variable for clarity

    FileDirUtils::createDirectory( testDir );
    std::ofstream testFile( testFilePath ); // Create a test file with proper variable name

    auto files = FileDirUtils::listFiles( testDir );
    EXPECT_EQ( files.size(), 1 );
    EXPECT_EQ( files[0], testFilePath );

    FileDirUtils::deleteDirectory( testDir );
}

TEST( FileDirUtilsTest, ListDirectories )
{
    std::string testDir = "test_dir";
    std::string subDir = "test_dir/sub_dir";

    FileDirUtils::createDirectory( testDir );
    FileDirUtils::createDirectory( subDir );

    auto dirs = FileDirUtils::listDirectories( testDir );
    EXPECT_EQ( dirs.size(), 1 );
    EXPECT_EQ( dirs[0], subDir );

    FileDirUtils::deleteDirectory( testDir );
}

TEST( FileDirUtilsTest, FileExists )
{
    std::string testFilePath = "test_file.txt"; // Renamed variable for clarity
    std::ofstream testFile( testFilePath ); // Create a test file with proper variable name

    EXPECT_TRUE( FileDirUtils::fileExists( testFilePath ) );

    fs::remove( testFilePath ); // Clean up
    EXPECT_FALSE( FileDirUtils::fileExists( testFilePath ) );
}

TEST( FileDirUtilsTest, DirectoryExists )
{
    std::string testDir = "test_dir";

    FileDirUtils::createDirectory( testDir );
    EXPECT_TRUE( FileDirUtils::directoryExists( testDir ) );

    FileDirUtils::deleteDirectory( testDir );
    EXPECT_FALSE( FileDirUtils::directoryExists( testDir ) );
}

int main( int argc, char **argv )
{
    Logger::SetPriority( LogLevel::TRACE );
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
