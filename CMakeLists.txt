cmake_minimum_required(VERSION 3.5)

project(AstragaramLibs)

# Enable testing and add tests
enable_testing()

find_package(GTest REQUIRED CONFIG)
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)

message(STATUS "CMake Version       : " ${CMAKE_VERSION})
message(STATUS "GTest Version       : " ${GTest_VERSION})
message(STATUS "Protobuf Version    : " ${Protobuf_VERSION})
message(STATUS "Boost Version       : " ${Boost_VERSION})
message(STATUS "CMAKE_SOURCE_DIR    : " ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_BINARY_DIR    : " ${CMAKE_BINARY_DIR})

#### Function to Generate Protobuf Files ######################################
function(generate_protobuf OUTPUT_VAR TYPE)
    cmake_parse_arguments(GEN_PROTO "" "" "FILES" ${ARGN})
    if(NOT GEN_PROTO_FILES)
        message(FATAL_ERROR "No protobuf files provided")
    endif()

    # Generate protobuf files
    protobuf_generate_cpp(
        ${OUTPUT_VAR}_PROTO_SRCS 
        ${OUTPUT_VAR}_PROTO_HDRS 
        ${GEN_PROTO_FILES}
        PROTOC_OUT_DIR ${CMAKE_BINARY_DIR}
    )

    # Print the generated files for debugging
    message(STATUS "Generated Protobuf ${OUTPUT_VAR}_PROTO_SRCS:")
    foreach(src_file ${${OUTPUT_VAR}_PROTO_SRCS})
        message(STATUS "    ${src_file}")
    endforeach()

    message(STATUS "Generated Protobuf ${OUTPUT_VAR}_PROTO_HDRS:")
    foreach(hdr_file ${${OUTPUT_VAR}_PROTO_HDRS})
        message(STATUS "    ${hdr_file}")
    endforeach()

    # Create a library for the generated Protobuf files
    if(${OUTPUT_VAR}_PROTO_SRCS)
        if(${TYPE} STREQUAL "static")
            add_library(${OUTPUT_VAR} STATIC ${${OUTPUT_VAR}_PROTO_SRCS})
        elseif(${TYPE} STREQUAL "shared")
            add_library(${OUTPUT_VAR} SHARED ${${OUTPUT_VAR}_PROTO_SRCS})
        else()
            message(FATAL_ERROR "Invalid library type: ${TYPE}")
        endif()
    else()
        message(FATAL_ERROR "No source files found for ${OUTPUT_VAR}")
    endif()

    # Include the generated headers correctly
    target_include_directories(${OUTPUT_VAR} PUBLIC ${CMAKE_BINARY_DIR})
    target_link_libraries(${OUTPUT_VAR} protobuf::libprotobuf)

    # Install the generated headers
    install(FILES ${${OUTPUT_VAR}_PROTO_HDRS} DESTINATION include)
endfunction()

#### Function to Add Custom Tests #############################################
function(add_custom_test NAME SOURCE)
    add_executable(${NAME} ${SOURCE})
    target_link_libraries(${NAME} AstragaramLibs)
    add_test(NAME ${NAME} COMMAND ${NAME})
endfunction()

#### Generate Protobuf Files ###################################################
# Generate protobuf libraries
generate_protobuf(protolib static FILES
    "${CMAKE_SOURCE_DIR}/ProtoFiles/NetworkInterface.proto"
    "${CMAKE_SOURCE_DIR}/ProtoFiles/person.proto"
)

#### AstragaramLibs Static Library #############################################

# Create a single static library
add_library(AstragaramLibs STATIC 
    Logger/Src/Logger.cpp
    # IPC/Src/ServerSocket.cpp
    # IPC/Src/ClientSocket.cpp
    # IPC/Src/Socket.cpp
    ThreadManager/Src/ThreadManager.cpp
    FileDirUtils/Src/FileDirUtils.cpp
    # StringUtils/Src/StringUtils.cpp
    # NetworkUtils/Src/NetworkUtils.cpp
)

target_include_directories(AstragaramLibs PUBLIC
    Logger/Src
    # IPC/Src
    ThreadManager/Src
    FileDirUtils/Src
    # StringUtils/Src
    # ${CMAKE_BINARY_DIR}/ProtoFiles
    # NetworkUtils/Src
)

target_link_libraries(AstragaramLibs
    Boost::system
    Boost::filesystem
    protobuf::libprotobuf
    GTest::gtest 
    GTest::gtest_main
    protolib
)

install(TARGETS AstragaramLibs DESTINATION lib)
install(FILES Logger/Src/Logger.h DESTINATION include)
# install(DIRECTORY IPC/Src/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ThreadManager/Src/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY FileDirUtils/Src/ DESTINATION include FILES_MATCHING PATTERN "*.h")
# install(DIRECTORY StringUtils/Src/ DESTINATION include FILES_MATCHING PATTERN "*.h")

#### Executables and Tests ###################################################
add_executable(TestLogger Logger/Src/Logger.cpp)
target_link_libraries(TestLogger AstragaramLibs)

# add_executable(simple_server IPC/simple_server_main.cpp)
# target_link_libraries(simple_server AstragaramLibs)

# add_executable(simple_client IPC/simple_client_main.cpp)
# target_link_libraries(simple_client AstragaramLibs)

add_executable(example_thread ThreadManager/example_thread.cpp)
target_link_libraries(example_thread AstragaramLibs)

add_executable(example_misc MiscUtils/MiscUtils.cpp)
target_link_libraries(example_misc AstragaramLibs)

# Using the custom function to add tests
# add_custom_test(test_Socket IPC/GTest/test_Socket.cpp)
# add_custom_test(test_sum GTest/test.cpp)
add_custom_test(test_ThreadManager ThreadManager/GTest/test_ThreadManager.cpp)
add_custom_test(test_FileDirUtils FileDirUtils/GTest/test_FileDirUtils.cpp)
# add_custom_test(test_StringUtils StringUtils/GTest/test_StringUtils.cpp)
# add_custom_test(test_ProtobufHelper GoogleProtocolBuffers/GTest/test_ProtobufHelper.cpp)
# add_custom_test(test_NetworkUtils NetworkUtils/GTest/test_NetworkUtils.cpp)
