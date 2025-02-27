#pragma once

#ifndef HEADER_FILE_NAME
#define HEADER_FILE_NAME

#include <stdio.h>
#include <mutex>
#include <ctime>
#include <iostream>
#include <unistd.h> // For getpid()

#define LOG_TRACE(Message, ...) (Logger::Trace(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_DEBUG(Message, ...) (Logger::Debug(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_INFO(Message, ...) (Logger::Info(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_WARN(Message, ...) (Logger::Warn(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_ERROR(Message, ...) (Logger::Error(__LINE__, __FILE__, Message, ##__VA_ARGS__))
#define LOG_CRITICAL(Message, ...) (Logger::Critical(__LINE__, __FILE__, Message, ##__VA_ARGS__))

enum LogLevel
{
    TRACE, DEBUG, INFO, WARN, ERROR, CRITICAL
};

class Logger
{
private:
    LogLevel priority = INFO;
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
        get_instance().log( "Trace", TRACE, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Debug( const char* message, Args... args )
    {
        get_instance().log( "Debug", DEBUG, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Info( const char* message, Args... args )
    {
        get_instance().log( "Info", INFO, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Warn( const char* message, Args... args )
    {
        get_instance().log( "Warn", WARN, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Error( const char* message, Args... args )
    {
        get_instance().log( "Error", ERROR, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Critical( const char* message, Args... args )
    {
        get_instance().log( "Critical", CRITICAL, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Trace( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Trace ", TRACE, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Debug( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Debug", DEBUG, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Info( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Info", INFO, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Warn( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Warn", WARN, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Error( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Error", ERROR, message, std::forward<Args>( args )... );
    }

    template<typename... Args>
    static void Critical( int line, const char* source_file, const char* message, Args... args )
    {
        get_instance().log( line, source_file, "Critical", CRITICAL, message, std::forward<Args>( args )... );
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
            std::tm* timestamp = std::localtime( &current_time );
            char buffer[80];
            strftime( buffer, 80, "%c", timestamp );

            std::scoped_lock lock( log_mutex );
            printf( "%s [%s][%d]: ", buffer, message_priority_str, getpid() );
            printf( message, std::forward<Args>( args )... );
            printf( "\n" );

            if( file )
            {
                fprintf( file, "%s [%s][%d]: ", buffer, message_priority_str, getpid() );
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
            std::tm* timestamp = std::localtime( &current_time );
            char buffer[80];
            strftime( buffer, 80, "%c", timestamp );

            std::scoped_lock lock( log_mutex );
            printf( "%s [%s][%d] %s:%d: ", buffer, message_priority_str, getpid(), source_file, line_number );
            printf( message, std::forward<Args>( args )... );
            printf( "\n" );

            if( file )
            {
                fprintf( file, "%s [%s][%d] %s:%d: ", buffer, message_priority_str, getpid(), source_file, line_number );
                fprintf( file, message, std::forward<Args>( args )... );
                fprintf( file, "\n" );
            }
        }
    }

    bool enable_file_output()
    {
        free_file();

        file = std::fopen( filepath, "a" );

        if( file == 0 )
        {
            return false;
        }

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
