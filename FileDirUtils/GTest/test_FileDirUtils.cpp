#include <filesystem>
#include <fstream>
#include "gtest/gtest.h"
#include "FileDirUtils.h"
#include "Logger.h"

#if __cplusplus >= 202002L
#include <filesystem>
namespace fs = std::filesystem;
#elif __cplusplus >= 201703L
#include <filesystem>
namespace fs = std::filesystem;
#elif __cplusplus >= 201402L
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

TEST( FileDirUtilsTest, CreateDeleteDirectory )
{
    std::string testDir = "test_dir1";

    // Create Directory
    EXPECT_TRUE( FileDirUtils::createDirectory( testDir ) );
    EXPECT_TRUE( FileDirUtils::directoryExists( testDir ) );

    // Delete Directory
    EXPECT_TRUE( FileDirUtils::deleteDirectory( testDir ) );
    EXPECT_FALSE( FileDirUtils::directoryExists( testDir ) );
}

TEST( FileDirUtilsTest, ListFiles )
{
    std::string testDir = "test_dir2";
#ifdef _WIN32
    std::string testFilePath = "test_dir2\\test_file.txt";
#else
    std::string testFilePath = "test_dir2/test_file.txt";
#endif
    EXPECT_TRUE( FileDirUtils::createDirectory( testDir ) );
    std::ofstream testFile( testFilePath );

    auto files = FileDirUtils::listFiles( testDir );
    EXPECT_EQ( files.size(), 1 );
    EXPECT_EQ( fs::path( files[0] ).string(), fs::path( testFilePath ).string() );

    FileDirUtils::deleteDirectory( testDir );
}

TEST( FileDirUtilsTest, ListDirectories )
{
    std::string testDir = "test_dir3";
#ifdef _WIN32
    std::string subDir = "test_dir3\\sub_dir";
#else
    std::string subDir = "test_dir3/sub_dir";
#endif
    EXPECT_TRUE( FileDirUtils::createDirectory( testDir ) );
    EXPECT_TRUE( FileDirUtils::createDirectory( subDir ) );

    auto dirs = FileDirUtils::listDirectories( testDir );
    EXPECT_EQ( dirs.size(), 1 );
    EXPECT_EQ( fs::path( dirs[0] ).string(), fs::path( subDir ).string() ); // Normalize paths

    EXPECT_TRUE( FileDirUtils::deleteDirectory( testDir ) );
}

TEST( FileDirUtilsTest, FileExists )
{
    std::string testFilePath = "test_file.txt";
    std::ofstream testFile( testFilePath ); // Create a test file
    testFile.close(); // Ensure the file is closed

    EXPECT_TRUE( FileDirUtils::fileExists( testFilePath ) );

    fs::remove( testFilePath ); // Clean up
    EXPECT_FALSE( FileDirUtils::fileExists( testFilePath ) );
}

TEST( FileDirUtilsTest, DirectoryExists )
{
    std::string testDir = "test_dir4";

    FileDirUtils::createDirectory( testDir );
    EXPECT_TRUE( FileDirUtils::directoryExists( testDir ) );

    FileDirUtils::deleteDirectory( testDir );
    EXPECT_FALSE( FileDirUtils::directoryExists( testDir ) );
}

int main( int argc, char **argv )
{
    Logger::SetPriority( LogLevel::LL_TRACE );
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
