cmake_minimum_required(VERSION 3.22)

message(INFO " Cmake Version : "${CMAKE_VERSION})
project(AstragaramLibs)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#### Logger ##################################################################
# add_library(Logger STATIC Logger/include/Logger.h)

add_executable(TestLogger 
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger/Logger.cpp 
)

target_include_directories(TestLogger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger/include/
)

# target_link_libraries(TestLogger PRIVATE Logger)

install(FILES Logger/include/Logger.h DESTINATION include)

#### IPC #####################################################################

add_library(IPCSocket STATIC
    IPC/ServerClient/Sources/ServerSocket.cpp
    IPC/ServerClient/Sources/ClientSocket.cpp
    IPC/ServerClient/Sources/Socket.cpp
)

target_include_directories(IPCSocket PUBLIC 
    IPC/ServerClient/Include/
    Logger/include/
)

install(TARGETS IPCSocket DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib)

add_executable(simple_server IPC/simple_server_main.cpp)
add_executable(simple_client IPC/simple_client_main.cpp)

target_link_libraries(simple_server IPCSocket)
target_link_libraries(simple_client IPCSocket)
