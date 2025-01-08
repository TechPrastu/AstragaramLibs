#include "ThreadManager.h"
#include <iostream>

// Constructor
ThreadManager::ThreadManager() = default;

// Destructor to join all threads
ThreadManager::~ThreadManager()
{
    joinAll();
}

// Add a new thread with a given function
void ThreadManager::addThread( std::function<void()> func )
{
    threads.emplace_back( [func]()
    {
        try
        {
            func();
        }
        catch( const std::exception& e )
        {
            std::cerr << "Exception in thread: " << e.what() << std::endl;
        }
        catch( ... )
        {
            std::cerr << "Unknown exception in thread" << std::endl;
        }
    } );
}

// Join all threads
void ThreadManager::joinAll()
{
    for( auto& thread : threads )
    {
        if( thread.joinable() )
        {
            try
            {
                thread.join();
            }
            catch( const std::exception& e )
            {
                std::cerr << "Exception while joining thread: " << e.what() << std::endl;
            }
            catch( ... )
            {
                std::cerr << "Unknown exception while joining thread" << std::endl;
            }
        }
    }
}
