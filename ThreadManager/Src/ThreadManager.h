#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <thread>
#include <vector>
#include <functional>
#include <iostream>
#include <exception>

class ThreadManager
{
public:
    // Constructor
    ThreadManager();

    // Destructor to join all threads
    ~ThreadManager();

    // Add a new thread with a given function
    void addThread( std::function<void()> func );

    // Join all threads
    void joinAll();

private:
    std::vector<std::thread> threads; // Container for thread objects
};

#endif // THREAD_MANAGER_H
