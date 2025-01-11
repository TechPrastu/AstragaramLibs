// Implementation of the ClientSocket class

#include "ClientSocket.h"
#include "SocketException.h"
#include "Logger.h"


ClientSocket::ClientSocket( std::string host, int port )
{
    Logger::Trace( "%s: host:%s, port:%d", __func__, host.c_str(), port );

    if( !Socket::create() )
    {
        Logger::Error( " %s Could not create client socket.", __func__ );
        throw SocketException( "Could not create client socket." );
    }
    if( !Socket::connect( host, port ) )
    {
        Logger::Error( " %s Could not connect to port.", __func__ );
        throw SocketException( "Could not connect to port." );
    }
}
ClientSocket::ClientSocket( std::string serverPath )
{
    Logger::Trace( "%s: serverPath:%s", __func__, serverPath.c_str() );

    if( !Socket::create( serverPath ) )
    {
        Logger::Error( " %s Could not create client socket.", __func__ );
        throw SocketException( "Could not create client socket." );
    }
    if( !Socket::connect( serverPath ) )
    {
        Logger::Error( " %s Could not connect to serverPath: %s", __func__, serverPath.c_str() );
        throw SocketException( "Could not connect to serverPath " + serverPath );
    }
}

const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
    if( ! Socket::send( s ) )
    {
        throw SocketException( "Could not write to socket." );
    }

    return *this;
}

const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
    if( ! Socket::recv( s ) )
    {
        throw SocketException( "Could not read from socket." );
    }

    return *this;
}
