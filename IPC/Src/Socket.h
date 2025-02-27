#ifndef SOCKET_H
#define SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <string>
#include <iostream>

const int BUFFER_LENGTH = 250;
const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 5;
const int MAXRECV = 500;

class Socket
{
public:
    Socket();
    virtual ~Socket();

    // Server initialization
    bool create();
    bool create( const std::string );
    bool bind( const int port );
    bool bind( const std::string );
    bool listen() const;
    bool accept( Socket & ) const;

    // Client initialization
    bool connect( const std::string host, const int port );
    bool connect( const std::string );

    // Data Transmission
    bool send( const std::string ) const;
    int recv( std::string & ) const;

    void set_non_blocking( const bool );

    bool is_valid() const
    {
        return m_sock != -1;
    }

private:
    int m_sock;
    sockaddr_in m_addr;
#ifdef _WIN32
    sockaddr_in serveraddr;  // Windows uses sockaddr_in for server addresses
#else
    sockaddr_un serveraddr;  // Unix-based systems use sockaddr_un for server addresses
#endif
};

#endif // SOCKET_H
