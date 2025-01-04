cmake_minimum_required(VERSION 3.22)

message(INFO " Cmake Version : "${CMAKE_VERSION})
project(AstragaramLibs)

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
install(FILES 
    IPC/Src/ClientSocket.h
    IPC/Src/ServerSocket.h
    IPC/Src/Socket.h
    IPC/Src/SocketException.h
    DESTINATION include
)

add_executable(simple_server IPC/simple_server_main.cpp)
add_executable(simple_client IPC/simple_client_main.cpp)
target_link_libraries(simple_server IPCSocket)
target_link_libraries(simple_client IPCSocket)

#### GTest #####################################################################

add_executable(test_sum GTest/test.cpp)
target_link_libraries(test_sum GTest::gtest GTest::gtest_main ${CONAN_LIBS})

add_test(NAME test_sum COMMAND test_sum)
