// StringUtils.h
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>
#include "Logger.h" // Include Logger

class StringUtils
{
public:
    static std::string toUpperCase( const std::string &input );
    static std::string toLowerCase( const std::string &input );
    static bool startsWith( const std::string &input, const std::string &prefix );
    static bool endsWith( const std::string &input, const std::string &suffix );
    static std::string trim( const std::string &input );
    static std::string replace( const std::string &input, const std::string &search, const std::string &replace );
    static std::vector<std::string> search( const std::string &input, const std::string &search );
    static std::string remove( const std::string &input, const std::string &search );
};

#endif // STRINGUTILS_H
