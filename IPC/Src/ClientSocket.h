// Definition of the ClientSocket class

#ifndef ClientSocket_class
#define ClientSocket_class

#include "Socket.h"


class ClientSocket : private Socket
{
public:

    ClientSocket( std::string host, int port );
#ifndef _WIN32
    ClientSocket( std::string serverPath );
#endif
    virtual ~ClientSocket() {};

    const ClientSocket& operator << ( const std::string& ) const;
    const ClientSocket& operator >> ( std::string& ) const;

};


#endif
