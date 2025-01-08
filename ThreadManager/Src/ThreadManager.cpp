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
void ThreadManager::addThread(std::function<void()> func)
{
    threads.emplace_back(func);
}

// Join all threads
void ThreadManager::joinAll()
{
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
