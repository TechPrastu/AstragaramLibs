cmake_minimum_required(VERSION 3.3)

project(AstragaramLibs)

# Enable testing and add tests 
enable_testing()

find_package(GTest REQUIRED CONFIG)
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED)

message(STATUS "CMake Version: "${CMAKE_VERSION})
message(STATUS "Protobuf Version: "${Protobuf_VERSION})

#### Logger ##################################################################

add_executable(TestLogger 
    Logger/Src/Logger.cpp 
)

target_include_directories(TestLogger PRIVATE
    Logger/Src
)

install(FILES Logger/Src/Logger.h DESTINATION include)

#### IPC ######################################################################

add_library(IPCSocket STATIC
    IPC/Src/ServerSocket.cpp
    IPC/Src/ClientSocket.cpp
    IPC/Src/Socket.cpp
)

target_include_directories(IPCSocket PUBLIC 
    IPC/Src
    Logger/Src
)

install(TARGETS IPCSocket DESTINATION lib)
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

install(TARGETS ThreadManager DESTINATION lib)
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

install(TARGETS FileDirUtils DESTINATION lib)
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

install(TARGETS StringUtils DESTINATION lib)
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

#### Google Protocol Buffers (Protobuf) #####################################################################
# Specify the path to generate the protobuf files 
set(PROTO_SRC_DIR "${CMAKE_SOURCE_DIR}/GoogleProtocolBuffers/GTest/ProtosFiles") 

# Generate protobuf files with specified output directory 
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
    ${PROTO_SRC_DIR}/person.proto
)

# Print the generated files for debugging
message(STATUS "Generated Protobuf PROTO_SRCS: ${PROTO_SRCS}")
message(STATUS "Generated Protobuf PROTO_HDRS: ${PROTO_HDRS}")

# Create a static library for the generated Protobuf files 
add_library(personProto STATIC ${PROTO_SRCS})

# Include the generated headers correctly
get_filename_component(PROTO_HDRS_DIR ${PROTO_HDRS} DIRECTORY)
target_include_directories(personProto PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(personProto protobuf::libprotobuf)

# Add the source files to create static lib


# Add the GTest for GoogleProtocolBuffers
add_executable(test_ProtobufHelper GoogleProtocolBuffers/GTest/test_ProtobufHelper.cpp)

target_include_directories(test_ProtobufHelper PUBLIC
    GoogleProtocolBuffers/Src
    ${PROTO_HDRS_DIR}
)

target_link_libraries(test_ProtobufHelper
    GTest::gtest 
    GTest::gtest_main
    personProto
)

add_test(NAME test_ProtobufHelper COMMAND test_ProtobufHelper)

#### NetworkUtils #################################################################

# Generate protobuf files
protobuf_generate_cpp(NetworkInterface_PROTO_SRCS NetworkInterface_PROTO_HDRS ${CMAKE_SOURCE_DIR}/NetworkUtils/ProtoFiles/NetworkInterface.proto)
# Print the generated files for debugging
message(STATUS "Generated Protobuf NetworkInterface_PROTO_SRCS: ${NetworkInterface_PROTO_SRCS}")
message(STATUS "Generated Protobuf NetworkInterface_PROTO_HDRS: ${NetworkInterface_PROTO_HDRS}")

# Create a static library for the generated Protobuf files 
add_library(NetworkInterfaceProto STATIC ${NetworkInterface_PROTO_SRCS})

# Include the generated headers correctly
get_filename_component(PNetworkInterface_PROTO_HDRS_DIR ${NetworkInterface_PROTO_HDRS} DIRECTORY)
target_include_directories(NetworkInterfaceProto PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries(NetworkInterfaceProto protobuf::libprotobuf)


add_library(NetworkUtils STATIC
    NetworkUtils/Src/NetworkUtils.cpp
)

target_include_directories(NetworkUtils PUBLIC
    NetworkUtils/Src/
    Logger/Src
    ${PNetworkInterface_PROTO_HDRS_DIR}
)

target_link_libraries(NetworkUtils boost::boost protobuf::libprotobuf NetworkInterfaceProto)

# Add the GTest for NetworkUtils
add_executable(test_NetworkUtils NetworkUtils/GTest/test_NetworkUtils.cpp)

target_include_directories(test_NetworkUtils PUBLIC
    NetworkUtils/Src
    ${PNetworkInterface_PROTO_HDRS_DIR}
)

target_link_libraries(test_NetworkUtils
    GTest::gtest 
    GTest::gtest_main
    boost::boost
    NetworkInterfaceProto
    NetworkUtils
)

add_test(NAME test_NetworkUtils COMMAND test_ProtobufHelper)

########################################################################################
