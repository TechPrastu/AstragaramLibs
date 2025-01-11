#include <gtest/gtest.h>
#include <thread>
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketException.h"
#include "Logger.h"


// Function to run the server using a port
void run_server_port()
{
    Logger::Info( "SendReceiveData (port): Set up server" );
    ServerSocket server( 8080 );
    ServerSocket new_sock;
    Logger::Info( "SendReceiveData (port): server.accept" );
    server.accept( new_sock, false );

    Logger::Info( "SendReceiveData (port): Server receives data" );
    std::string recv_data;
    ASSERT_NO_THROW( new_sock >> recv_data );
    EXPECT_EQ( "Hello, Server!", recv_data );
}

// Function to run the client using a port
void run_client_port()
{
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    Logger::Info( "SendReceiveData (port): Set up client" );
    ClientSocket client( "127.0.0.1", 8080 );

    Logger::Info( "SendReceiveData (port): Client sends data" );
    std::string send_data = "Hello, Server!";
    ASSERT_NO_THROW( client << send_data );
}

// Function to run the server using a socket file
void run_server_socket()
{
    Logger::Info( "SendReceiveData (socket): Set up server" );
    ServerSocket server( "/tmp/server.sock" );
    ServerSocket new_sock;
    Logger::Info( "SendReceiveData (socket): server.accept" );
    server.accept( new_sock, false );

    Logger::Info( "SendReceiveData (socket): Server receives data" );
    std::string recv_data;
    ASSERT_NO_THROW( new_sock >> recv_data );
    EXPECT_EQ( "Hello, Server!", recv_data );
}

// Function to run the client using a socket file
void run_client_socket()
{
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    Logger::Info( "SendReceiveData (socket): Set up client" );
    ClientSocket client( "/tmp/server.sock" );

    Logger::Info( "SendReceiveData (socket): Client sends data" );
    std::string send_data = "Hello, Server!";
    ASSERT_NO_THROW( client << send_data );
}

// Test case for sending and receiving data using a port
TEST( SocketCommunicationTest, SendReceiveDataPort )
{
    std::thread server_thread( run_server_port );
    std::thread client_thread( run_client_port );

    server_thread.join();
    client_thread.join();
}

// Test case for sending and receiving data using a socket file
TEST( SocketCommunicationTest, SendReceiveDataSocket )
{
    std::thread server_thread( run_server_socket );
    std::thread client_thread( run_client_socket );

    server_thread.join();
    client_thread.join();
}

// Test case for ServerSocket creation with port
TEST( ServerSocketTest, CreateWithPort )
{
    ASSERT_NO_THROW( ServerSocket server( 8080 ) );
}

// Test case for ServerSocket creation with serverPath
TEST( ServerSocketTest, CreateWithServerPath )
{
    unlink( "/tmp/server.sock" );
    ASSERT_NO_THROW( ServerSocket server( "/tmp/server.sock" ) );
}

// // Test case for ClientSocket creation with host and port
// TEST( ClientSocketTest, CreateWithHostAndPort )
// {
//     ASSERT_NO_THROW( ClientSocket client( "127.0.0.1", 30000 ) );
// }

// // Test case for ClientSocket creation with serverPath
// TEST( ClientSocketTest, CreateWithServerPath )
// {
//     ASSERT_NO_THROW( ClientSocket client( "/tmp/server.sock" ) );
// }

// Test case for SocketException
TEST( SocketExceptionTest, ThrowSocketException )
{
    try
    {
        throw SocketException( "Test exception" );
    }
    catch( SocketException &e )
    {
        EXPECT_EQ( std::string( e.what() ), "Test exception" );
    }
}

int main( int argc, char **argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
