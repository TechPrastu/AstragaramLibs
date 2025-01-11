#pragma once

// FileDirUtils.h
#ifndef FILEDIRUTILS_H
#define FILEDIRUTILS_H

#include <string>
#include <vector>

class FileDirUtils
{
public:
    static bool createDirectory( const std::string &path );
    static bool deleteDirectory( const std::string &path );
    static std::vector<std::string> listFiles( const std::string &path );
    static std::vector<std::string> listDirectories( const std::string &path );
    static bool fileExists( const std::string &path );
    static bool directoryExists( const std::string &path );
};

#endif // FILEDIRUTILS_H
