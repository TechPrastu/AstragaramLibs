// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"
#include "Logger.h"

ServerSocket::ServerSocket( int port )
{
    Logger::Trace( "%s: port:%d", __func__, port );
    if( !Socket::create() )
    {
        Logger::Error( "%s: Could not create server socket.", __func__ );
        throw SocketException( "Could not create server socket." );
    }

    if( !Socket::bind( port ) )
    {
        Logger::Error( "%s: Could not bind to port.", __func__ );
        throw SocketException( "Could not bind to port." );
    }

    if( !Socket::listen() )
    {
        Logger::Error( "%s: Could not listen to socket.", __func__ );
        throw SocketException( "Could not listen to socket." );
    }
}

ServerSocket::ServerSocket( std::string serverPath )
{
    Logger::Trace( "%s: serverPath:%s", __func__, serverPath.c_str() );

    // Ensure the socket file does not already exist
    if( unlink( serverPath.c_str() ) == 0 )
    {
        Logger::Info( "Removed existing socket file: %s", serverPath.c_str() );
    }

    if( !Socket::create( serverPath ) )
    {
        Logger::Error( "%s: Could not create server socket.", __func__ );
        throw SocketException( "Could not create server socket." );
    }

    if( !Socket::bind( serverPath ) )
    {
        Logger::Error( "%s: Could not bind to serverPath.", __func__ );
        throw SocketException( "Could not bind to serverPath." );
    }

    if( !Socket::listen() )
    {
        Logger::Error( "%s: Could not listen to socket.", __func__ );
        throw SocketException( "Could not listen to socket." );
    }
}

ServerSocket::~ServerSocket()
{
    Logger::Trace( "%s", __func__ );
}

const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
    if( ! Socket::send( s ) )
    {
        throw SocketException( "Could not write to socket." );
    }

    return *this;

}

const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
    if( ! Socket::recv( s ) )
    {
        throw SocketException( "Could not read from socket." );
    }

    return *this;
}

void ServerSocket::accept( ServerSocket& sock, const bool &blocking )
{
    Logger::Trace( "%s", __func__ );

    if( ! Socket::accept( sock ) )
    {
        throw SocketException( "Could not accept socket." );
    }

    Socket::set_non_blocking( blocking );
}
