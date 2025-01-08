#include <iostream>
#include "ThreadManager.h"

// Example function to be executed by the thread
void exampleFunction( int threadID )
{
    std::cout << "Hello from thread " << threadID << std::endl;
}

int main()
{
    ThreadManager manager;

    // Add threads to the manager
    for( int i = 0; i < 5; ++i )
    {
        manager.addThread( [i]()
        {
            exampleFunction( i );
        } );
    }

    // Join all threads
    manager.joinAll();

    std::cout << "All threads have finished execution." << std::endl;
    return 0;
}
