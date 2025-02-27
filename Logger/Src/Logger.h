#pragma once

#ifndef HEADER_FILE_NAME
#define HEADER_FILE_NAME

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <mutex>
#include <ctime>
#include <iostream>

#define LOG_TRACE(Message, ...) (Logger::Trace(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_DEBUG(Message, ...) (Logger::Debug(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_INFO(Message, ...) (Logger::Info(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_WARN(Message, ...) (Logger::Warn(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_ERROR(Message, ...) (Logger::Error(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_CRITICAL(Message, ...) (Logger::Critical(__LINE__, __FILE__, Message, ##__VA_ARGS__))

enum LogLevel
{
    LL_TRACE,        // LogLevel Trace
    LL_DEBUG,        // LogLevel Debug
    LL_INFO,         // LogLevel Information
    LL_WARN,         // LogLevel Warning
    LL_ERROR,        // LogLevel Error
    LL_CRITICAL      // LogLevel Critical
};

class Logger
{
private:
    LogLevel priority = LL_INFO;
    std::mutex log_mutex;
    const char* filepath = 0;
    FILE* file = 0;

public:
    static void SetPriority( LogLevel new_priority )
    {
        get_instance().priority = new_priority;
    }

    static void EnableFileOutput()
    {
        Logger& logger_instance = get_instance();
        logger_instance.filepath = "log.txt";
        logger_instance.enable_file_output();
    }

    static void EnableFileOutput( const char* new_filepath )
    {
        Logger& logger_instance = get_instance();
        logger_instance.filepath = new_filepath;
        logger_instance.enable_file_output();
    }

    template<typename... Args>
    static void Trace( const char* message, Args... args )
    {
        get_instance().log( "Trace", LL_TRACE, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Debug( const char* message, Args... args )
    {
        get_instance().log( "Debug", LL_DEBUG, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Info( const char* message, Args... args )
    {
        get_instance().log( "Info", LL_INFO, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Warn( const char* message, Args... args )
    {
        get_instance().log( "Warn", LL_WARN, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Error( const char* message, Args... args )
    {
        get_instance().log( "Error", LL_ERROR, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Critical( const char* message, Args... args )
    {
        get_instance().log( "Critical", LL_CRITICAL, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Trace( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Trace", LL_TRACE, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Debug( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Debug", LL_DEBUG, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Info( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Info", LL_INFO, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Warn( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Warn", LL_WARN, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Error( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Error", LL_ERROR, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Critical( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Critical", LL_CRITICAL, message, std::forward<Args>( args )... );
    }

private:
    Logger() {}

    Logger( const Logger& ) = delete;
    Logger& operator=( const Logger& ) = delete;

    ~Logger()
    {
        free_file();
    }

    static Logger& get_instance()
    {
        static Logger logger;
        return logger;
    }

    template<typename... Args>
    void log( const char* message_priority_str, LogLevel message_priority, const char* message, Args... args )
    {
        if( priority <= message_priority )
        {
            std::time_t current_time = std::time( 0 );
            char buffer[80];
#ifdef _WIN32
            std::tm timestamp;
            localtime_s( &timestamp, &current_time );
            strftime( buffer, 80, "%c", &timestamp );
            std::lock_guard<std::mutex> lock( log_mutex );
            printf( "%s [%s][%lu]: ", buffer, message_priority_str, GetCurrentProcessId() );
#else
            std::tm* timestamp = std::localtime( &current_time );
            strftime( buffer, 80, "%c", timestamp );
            std::scoped_lock lock( log_mutex );
            printf( "%s [%s][%d]: ", buffer, message_priority_str, getpid() );
#endif
            printf( message, std::forward<Args>( args )... );
            printf( "\n" );

            if( file )
            {
#ifdef _WIN32
                fprintf( file, "%s [%s][%lu]: ", buffer, message_priority_str, GetCurrentProcessId() );
#else
                fprintf( file, "%s [%s][%d]: ", buffer, message_priority_str, getpid() );
#endif
                fprintf( file, message, std::forward<Args>( args )... );
                fprintf( file, "\n" );
            }
        }
    }

    template<typename... Args>
    void log( int line_number, const char* source_file, const char* message_priority_str, LogLevel message_priority, const char* message, Args... args )
    {
        if( priority <= message_priority )
        {
            std::time_t current_time = std::time( 0 );
            char buffer[80];
#ifdef _WIN32
            std::tm timestamp;
            localtime_s( &timestamp, &current_time );
            strftime( buffer, 80, "%c", &timestamp );
            std::lock_guard<std::mutex> lock( log_mutex );
            printf( "%s [%s][%lu] %s:%d: ", buffer, message_priority_str, GetCurrentProcessId(), source_file, line_number );
#else
            std::tm* timestamp = std::localtime( &current_time );
            strftime( buffer, 80, "%c", timestamp );
            std::scoped_lock lock( log_mutex );
            printf( "%s [%s][%d] %s:%d: ", buffer, message_priority_str, getpid(), source_file, line_number );
#endif
            printf( message, std::forward<Args>( args )... );
            printf( "\n" );

            if( file )
            {
#ifdef _WIN32
                fprintf( file, "%s [%s][%lu] %s:%d: ", buffer, message_priority_str, GetCurrentProcessId(), source_file, line_number );
#else
                fprintf( file, "%s [%s][%d] %s:%d: ", buffer, message_priority_str, getpid(), source_file, line_number );
#endif
                fprintf( file, message, std::forward<Args>( args )... );
                fprintf( file, "\n" );
            }
        }
    }

    bool enable_file_output()
    {
        free_file();
#ifdef _WIN32
        errno_t err = fopen_s( &file, filepath, "a" );
        if( err != 0 )
        {
            char errMsg[256];
            strerror_s( errMsg, sizeof( errMsg ), err );
            printf( "Error opening file: %s\n", errMsg );
            return false;
        }
#else
        file = std::fopen( filepath, "a" );
        if( file == 0 )
        {
            return false;
        }
#endif
        return true;
    }

    void free_file()
    {
        if( file )
        {
            fclose( file );
            file = 0;
        }
    }
};

#endif
