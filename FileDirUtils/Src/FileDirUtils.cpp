// FileDirUtils.cpp
#include "FileDirUtils.h"
#include "Logger.h"
#include <filesystem>
#include <locale>
#include <codecvt>

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

std::wstring stringToWString( const std::string& str )
{
    Logger::Trace( "%s: str:%s", __func__, str.c_str() );
#ifdef _WIN32
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes( str );
#else
    return std::wstring( str.begin(), str.end() );
#endif
}

bool FileDirUtils::createDirectory( const std::string &path )
{
    Logger::Trace( "%s: path:%s", __func__, path.c_str() );
    try
    {
        std::wstring wpath = stringToWString( path );
        if( fs::create_directory( wpath ) )
        {
            Logger::Trace( "Directory created: %s", path.c_str() );
            return true;
        }
        else
        {
            Logger::Trace( "Directory already exists: %s", path.c_str() );
            return false;
        }
    }
    catch( const fs::filesystem_error &e )
    {
        Logger::Error( "Error: %s", e.what() );
        return false;
    }
}

bool FileDirUtils::deleteDirectory( const std::string &path )
{
    Logger::Trace( "%s: path:%s", __func__, path.c_str() );
    try
    {
        std::wstring wpath = stringToWString( path );
        if( fs::remove_all( wpath ) )
        {
            Logger::Trace( "Directory deleted: %s", path.c_str() );
            return true;
        }
        else
        {
            Logger::Trace( "Directory not found: %s", path.c_str() );
            return false;
        }
    }
    catch( const fs::filesystem_error &e )
    {
        Logger::Error( "Error: %s", e.what() );
        return false;
    }
}

std::vector<std::string> FileDirUtils::listFiles( const std::string &path )
{
    Logger::Trace( "%s: path:%s", __func__, path.c_str() );
    std::vector<std::string> files;
    try
    {
        std::wstring wpath = stringToWString( path );
        for( const auto &entry : fs::directory_iterator( wpath ) )
        {
            if( fs::is_regular_file( entry.status() ) )
            {
                files.push_back( entry.path().string() );
            }
        }
    }
    catch( const fs::filesystem_error &e )
    {
        Logger::Error( "Error: %s", e.what() );
    }
    return files;
}

std::vector<std::string> FileDirUtils::listDirectories( const std::string &path )
{
    Logger::Trace( "%s: path:%s", __func__, path.c_str() );
    std::vector<std::string> dirs;
    try
    {
        std::wstring wpath = stringToWString( path );
        for( const auto &entry : fs::directory_iterator( wpath ) )
        {
            if( fs::is_directory( entry.status() ) )
            {
                dirs.push_back( entry.path().string() );
            }
        }
    }
    catch( const fs::filesystem_error &e )
    {
        Logger::Error( "Error: %s", e.what() );
    }
    return dirs;
}

bool FileDirUtils::fileExists( const std::string &path )
{
    Logger::Trace( "%s: path:%s", __func__, path.c_str() );
    std::wstring wpath = stringToWString( path );
    return fs::exists( wpath ) && fs::is_regular_file( wpath );
}

bool FileDirUtils::directoryExists( const std::string &path )
{
    Logger::Trace( "%s: path:%s", __func__, path.c_str() );
    std::wstring wpath = stringToWString( path );
    return fs::exists( wpath ) && fs::is_directory( wpath );
}
