// FileDirUtils.cpp
#include "FileDirUtils.h"
#include "Logger.h"
#include <filesystem>

namespace fs = std::filesystem;

bool FileDirUtils::createDirectory( const std::string &path )
{
    try
    {
        if( fs::create_directory( path ) )
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
    try
    {
        if( fs::remove_all( path ) )
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
    std::vector<std::string> files;
    try
    {
        for( const auto &entry : fs::directory_iterator( path ) )
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
    std::vector<std::string> dirs;
    try
    {
        for( const auto &entry : fs::directory_iterator( path ) )
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
    return fs::exists( path ) && fs::is_regular_file( path );
}

bool FileDirUtils::directoryExists( const std::string &path )
{
    return fs::exists( path ) && fs::is_directory( path );
}
