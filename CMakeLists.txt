cmake_minimum_required(VERSION 3.3)

message(INFO " Cmake Version : "${CMAKE_VERSION})
project(AstragaramLibs)
# Enable testing and add tests 
enable_testing()

find_package(GTest REQUIRED CONFIG)

#### Logger ##################################################################

add_executable(TestLogger 
    Logger/Src/Logger.cpp 
)

target_include_directories(TestLogger PRIVATE
    Logger/Src
)

install(FILES Logger/Src/Logger.h DESTINATION include)

#### IPC #####################################################################

add_library(IPCSocket STATIC
    IPC/Src/ServerSocket.cpp
    IPC/Src/ClientSocket.cpp
    IPC/Src/Socket.cpp
)

target_include_directories(IPCSocket PUBLIC 
    IPC/Src
    Logger/Src
)

install(TARGETS IPCSocket DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib)
# Install header files 
install(DIRECTORY IPC/Src/ 
        DESTINATION 
        include FILES_MATCHING PATTERN "*.h"
)

add_executable(simple_server IPC/simple_server_main.cpp)
add_executable(simple_client IPC/simple_client_main.cpp)
target_link_libraries(simple_server IPCSocket)
target_link_libraries(simple_client IPCSocket)

add_executable(test_Socket IPC/GTest/test_Socket.cpp)
target_link_libraries(test_Socket 
    GTest::gtest 
    GTest::gtest_main
    IPCSocket
)

add_test(NAME test_Socket COMMAND test_Socket)

#### GTest #####################################################################

add_executable(test_sum GTest/test.cpp)
target_link_libraries(test_sum GTest::gtest GTest::gtest_main ${CONAN_LIBS})

add_test(NAME test_sum COMMAND test_sum)

#### ThreadManager #####################################################################

add_library(ThreadManager STATIC
    ThreadManager/Src/ThreadManager.cpp
)

target_include_directories(ThreadManager PUBLIC
    ThreadManager/Src
    Logger/Src
)

install(TARGETS ThreadManager DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib)
install(DIRECTORY ThreadManager/Src/ 
        DESTINATION 
        include FILES_MATCHING PATTERN "*.h"
)

add_executable(example_thread ThreadManager/example_thread.cpp)
target_link_libraries(example_thread ThreadManager)

add_executable(test_ThreadManager ThreadManager/GTest/test_ThreadManager.cpp)
target_link_libraries(test_ThreadManager 
    GTest::gtest 
    GTest::gtest_main
    ThreadManager
)

add_test(NAME test_ThreadManager COMMAND test_ThreadManager)

#### FileDirUtils #####################################################################
# Add the source files
add_library(FileDirUtils STATIC
    FileDirUtils/Src/FileDirUtils.cpp
)

target_include_directories(FileDirUtils PUBLIC
    FileDirUtils/Src
    Logger/Src
)

install(TARGETS FileDirUtils DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib)
install(DIRECTORY 
        FileDirUtils/Src/
        DESTINATION 
        include FILES_MATCHING PATTERN "*.h"
)

# Add the GTest executable
add_executable(test_FileDirUtils FileDirUtils/GTest/test_FileDirUtils.cpp)
target_link_libraries(test_FileDirUtils 
    GTest::gtest 
    GTest::gtest_main
    FileDirUtils
)

add_test(NAME test_FileDirUtils COMMAND test_FileDirUtils)

#### StringUtils #####################################################################
# Add the source files
add_library(StringUtils STATIC
    StringUtils/Src/StringUtils.cpp
)

target_include_directories(StringUtils PUBLIC
    StringUtils/Src
    Logger/Src
)

install(TARGETS StringUtils DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib)
install(DIRECTORY 
        StringUtils/Src/
        DESTINATION 
        include FILES_MATCHING PATTERN "*.h"
)

# Add the GTest executable
add_executable(test_StringUtils StringUtils/GTest/test_StringUtils.cpp)
target_link_libraries(test_StringUtils 
    GTest::gtest 
    GTest::gtest_main
    StringUtils
)

add_test(NAME test_StringUtils COMMAND test_StringUtils)

########################################################################################