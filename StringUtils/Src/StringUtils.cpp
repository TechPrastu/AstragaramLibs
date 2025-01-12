// StringUtils.cpp
#include "StringUtils.h"
#include <algorithm>
#include <cctype>
#include <locale>
#include <stdexcept>

std::string StringUtils::toUpperCase( const std::string &input )
{
    try
    {
        Logger::Trace( "Converting to upper case for input: %s", input.c_str() );

        std::string result = input;
        std::transform( result.begin(), result.end(), result.begin(), ::toupper );
        return result;
    }
    catch( const std::exception &e )
    {
        Logger::Error( "toUpperCase failed: %s", e.what() );
        throw;
    }
}

std::string StringUtils::toLowerCase( const std::string &input )
{
    try
    {
        Logger::Trace( "Converting to lower case for input: %s", input.c_str() );

        std::string result = input;
        std::transform( result.begin(), result.end(), result.begin(), ::tolower );
        return result;
    }
    catch( const std::exception &e )
    {
        Logger::Error( "toLowerCase failed: %s", e.what() );
        throw;
    }
}

bool StringUtils::startsWith( const std::string &input, const std::string &prefix )
{
    try
    {
        Logger::Trace( "Checking if input: %s starts with prefix: %s", input.c_str(), prefix.c_str() );

        return input.substr( 0, prefix.size() ) == prefix;
    }
    catch( const std::exception &e )
    {
        Logger::Error( "startsWith failed: %s", e.what() );
        throw;
    }
}

bool StringUtils::endsWith( const std::string &input, const std::string &suffix )
{
    try
    {
        Logger::Trace( "Checking if input: %s ends with suffix: %s", input.c_str(), suffix.c_str() );

        if( input.length() >= suffix.length() )
        {
            return input.compare( input.length() - suffix.length(), suffix.length(), suffix ) == 0;
        }
        return false;
    }
    catch( const std::exception &e )
    {
        Logger::Error( "endsWith failed: %s", e.what() );
        throw;
    }
}

std::string StringUtils::trim( const std::string &input )
{
    try
    {
        Logger::Trace( "Trimming input: %s", input.c_str() );

        auto start = input.begin();
        while( start != input.end() && std::isspace( *start ) )
        {
            start++;
        }

        auto end = input.end();
        do
        {
            end--;
        }
        while( std::distance( start, end ) > 0 && std::isspace( *end ) );

        return std::string( start, end + 1 );
    }
    catch( const std::exception &e )
    {
        Logger::Error( "trim failed: %s", e.what() );
        throw;
    }
}

std::string StringUtils::replace( const std::string &input, const std::string &search, const std::string &replace )
{
    try
    {
        Logger::Trace( "Replacing in input: %s search: %s with replace: %s", input.c_str(), search.c_str(), replace.c_str() );

        std::string result = input;
        size_t pos = 0;
        while( ( pos = result.find( search, pos ) ) != std::string::npos )
        {
            result.replace( pos, search.length(), replace );
            pos += replace.length();
        }
        return result;
    }
    catch( const std::exception &e )
    {
        Logger::Error( "replace failed: %s", e.what() );
        throw;
    }
}

std::vector<std::string> StringUtils::search( const std::string &input, const std::string &search )
{
    try
    {
        Logger::Trace( "Searching in input: %s for search: %s", input.c_str(), search.c_str() );

        std::vector<std::string> result;
        size_t pos = input.find( search );
        while( pos != std::string::npos )
        {
            result.push_back( input.substr( pos, search.length() ) );
            pos = input.find( search, pos + search.length() );
        }
        return result;
    }
    catch( const std::exception &e )
    {
        Logger::Error( "search failed: %s", e.what() );
        throw;
    }
}

std::string StringUtils::remove( const std::string &input, const std::string &search )
{
    try
    {
        Logger::Trace( "Removing in input: %s search: %s", input.c_str(), search.c_str() );

        std::string result = input;
        size_t pos = 0;
        while( ( pos = result.find( search, pos ) ) != std::string::npos )
        {
            result.erase( pos, search.length() );
        }
        return result;
    }
    catch( const std::exception &e )
    {
        Logger::Error( "remove failed: %s", e.what() );
        throw;
    }
}
