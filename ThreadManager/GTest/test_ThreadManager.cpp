#include <gtest/gtest.h>
#include "ThreadManager.h"

// Test if threads are created and executed correctly
TEST(ThreadManagerTest, AddAndExecuteThreads)
{
    ThreadManager manager;
    std::vector<int> results(5, 0);

    auto threadFunction = [&](int threadID) {
        results[threadID] = threadID * 2; // Just a simple operation
    };

    // Add threads to the manager
    for (int i = 0; i < 5; ++i) {
        manager.addThread([&, i]() {
            threadFunction(i);
        });
    }

    // Join all threads
    manager.joinAll();

    // Verify results
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(results[i], i * 2);
    }
}

// Test if joinAll works correctly with no threads
TEST(ThreadManagerTest, JoinAllWithoutThreads)
{
    ThreadManager manager;
    ASSERT_NO_THROW(manager.joinAll());
}

// Test if the destructor joins threads correctly
TEST(ThreadManagerTest, DestructorJoinsThreads)
{
    std::vector<int> results(5, 0);
    {
        ThreadManager manager;
        auto threadFunction = [&](int threadID) {
            results[threadID] = threadID * 3; // Another simple operation
        };

        // Add threads to the manager
        for (int i = 0; i < 5; ++i) {
            manager.addThread([&, i]() {
                threadFunction(i);
            });
        }
    } // Destructor should be called here

    // Verify results
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(results[i], i * 3);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
