#include "NetworkUtils.h"
#include "Logger.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <boost/bind/bind.hpp>

unsigned short NetworkUtils::InCksum( unsigned short *addr, int len )
{
    int sum = 0;
    unsigned short answer = 0;
    unsigned short *w = addr;
    int nleft = len;

    while( nleft > 1 )
    {
        sum += *w++;
        nleft -= 2;
    }

    if( nleft == 1 )
    {
        *( unsigned char * )( &answer ) = *( unsigned char * )w;
        sum += answer;
    }

    sum = ( sum >> 16 ) + ( sum & 0xFFFF );
    sum += ( sum >> 16 );
    answer = ~sum;

    return answer;
}

std::string NetworkUtils::GetSubnet( const std::string &ip, const std::string &mask )
{
    struct in_addr ip_addr, mask_addr, subnet_addr;
    inet_pton( AF_INET, ip.c_str(), &ip_addr );
    inet_pton( AF_INET, mask.c_str(), &mask_addr );
    subnet_addr.s_addr = ip_addr.s_addr & mask_addr.s_addr;

    char subnet[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &subnet_addr, subnet, sizeof( subnet ) );

    LOG_INFO( "Calculated subnet: %s", subnet );
    return std::string( subnet );
}

int NetworkUtils::GetCIDR( const std::string &mask )
{
    struct in_addr mask_addr;
    inet_pton( AF_INET, mask.c_str(), &mask_addr );

    unsigned long mask_long = ntohl( mask_addr.s_addr );
    int cidr = 0;
    while( mask_long )
    {
        cidr += ( mask_long & 1 );
        mask_long >>= 1;
    }

    LOG_INFO( "Calculated CIDR: %d", cidr );
    return cidr;
}

bool NetworkUtils::IsHostAliveICMP( const std::string &ip )
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    inet_pton( AF_INET, ip.c_str(), &addr.sin_addr );

    int sock = socket( AF_INET, SOCK_RAW, IPPROTO_ICMP );
    if( sock < 0 )
    {
        LOG_ERROR( "Error creating socket" );
        return false;
    }

    struct icmp icmp_hdr;
    std::memset( &icmp_hdr, 0, sizeof( icmp_hdr ) );
    icmp_hdr.icmp_type = ICMP_ECHO;
    icmp_hdr.icmp_code = 0;
    icmp_hdr.icmp_cksum = 0;
    icmp_hdr.icmp_seq = 1;
    icmp_hdr.icmp_id = getpid();

    icmp_hdr.icmp_cksum = InCksum( ( unsigned short * )&icmp_hdr, sizeof( icmp_hdr ) );

    if( sendto( sock, &icmp_hdr, sizeof( icmp_hdr ), 0, ( struct sockaddr * )&addr, sizeof( addr ) ) <= 0 )
    {
        LOG_ERROR( "Error sending ICMP echo" );
        close( sock );
        return false;
    }

    char buffer[1024];
    struct sockaddr_in recv_addr;
    socklen_t addr_len = sizeof( recv_addr );
    if( recvfrom( sock, buffer, sizeof( buffer ), 0, ( struct sockaddr * )&recv_addr, &addr_len ) <= 0 )
    {
        close( sock );
        LOG_WARN( "No response to ICMP echo from: %s", ip.c_str() );
        return false;
    }

    close( sock );
    LOG_INFO( "Received ICMP echo reply from: %s", ip.c_str() );
    return true;
}

std::vector<NetworkUtils::NetworkInterface> NetworkUtils::GetNetworkInterfaces()
{
    struct ifaddrs *interfaces = nullptr;
    std::vector<NetworkInterface> result;

    if( getifaddrs( &interfaces ) == 0 )
    {
        for( struct ifaddrs *ifa = interfaces; ifa != nullptr; ifa = ifa->ifa_next )
        {
            if( ifa->ifa_addr != nullptr && ifa->ifa_addr->sa_family == AF_INET && strcmp( ifa->ifa_name, "lo" ) != 0 )
            {
                NetworkInterface iface;
                iface.name = ifa->ifa_name;

                void *addr = &( ( ( struct sockaddr_in * )ifa->ifa_addr )->sin_addr );
                char ip[INET_ADDRSTRLEN];
                inet_ntop( AF_INET, addr, ip, sizeof( ip ) );
                iface.ip4 = ip;

                void *netmask = &( ( ( struct sockaddr_in * )ifa->ifa_netmask )->sin_addr );
                char mask[INET_ADDRSTRLEN];
                inet_ntop( AF_INET, netmask, mask, sizeof( mask ) );
                iface.subnetMask = mask;

                iface.isUp = ifa->ifa_flags & IFF_UP;
                result.push_back( iface );

                LOG_INFO( "Found network interface: %s, IP: %s, Subnet Mask: %s, Is Up: %d", iface.name.c_str(), iface.ip4.c_str(), iface.subnetMask.c_str(), iface.isUp );
            }
        }
        freeifaddrs( interfaces );
    }
    else
    {
        LOG_ERROR( "Error getting network interfaces" );
    }

    return result;
}

std::vector<NetworkInterfaceProto> NetworkUtils::GetNetworkInterfacesProto()
{
    std::vector<NetworkInterface> interfaces = GetNetworkInterfaces();
    std::vector<NetworkInterfaceProto> protoInterfaces;

    for( const auto &iface : interfaces )
    {
        NetworkInterfaceProto proto;
        proto.set_name( iface.name );
        proto.set_ip4( iface.ip4 );
        proto.set_ip6( iface.ip6 );
        proto.set_subnetmask( iface.subnetMask );
        proto.set_isup( iface.isUp );
        protoInterfaces.push_back( proto );

        LOG_INFO( "Added network interface proto: %s", iface.name.c_str() );
    }

    return protoInterfaces;
}

bool NetworkUtils::IsHostAlive( const std::string &ip, const std::vector<int> &ports, int timeout )
{
    boost::asio::io_context io_context;
    for( int port : ports )
    {
        Pinger pinger( io_context, ip, port, timeout );
        io_context.run();
        if( pinger.IsAlive() )
        {
            LOG_INFO( "Host is alive: %s:%d", ip.c_str(), port );
            return true;
        }
    }
    LOG_WARN( "Host is not alive: %s", ip.c_str() );
    return false;
}

bool NetworkUtils::CheckURLReachable( const std::string &url, int timeout )
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::resolver resolver( io_context );
    boost::asio::ip::tcp::resolver::query query( url, "80" );
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );

    boost::asio::ip::tcp::socket socket( io_context );
    boost::system::error_code ec;
    boost::asio::connect( socket, iterator, ec );

    if( ec )
    {
        LOG_WARN( "URL is not reachable: %s", url.c_str() );
    }
    else
    {
        LOG_INFO( "URL is reachable: %s", url.c_str() );
    }

    return !ec;
}

std::vector<std::string> NetworkUtils::FindAvailableDevices( const std::string &subnet, int cidr, const std::vector<int> &ports )
{
    std::vector<std::string> activeDevices;
    std::vector<std::thread> threads;

    for( int i = 1; i <= 254; ++i )
    {
        threads.emplace_back( [&activeDevices, &subnet, i, &ports]()
        {
            std::string target_ip = subnet + "." + std::to_string( i );
            if( NetworkUtils::IsHostAlive( target_ip, ports ) || NetworkUtils::IsHostAliveICMP( target_ip ) )
            {
                LOG_INFO( "Device found: %s", target_ip.c_str() );
                activeDevices.push_back( target_ip );
            }
        } );
    }

    for( auto &thread : threads )
    {
        if( thread.joinable() )
        {
            thread.join();
        }
    }

    return activeDevices;
}

std::vector<int> NetworkUtils::FindOpenPorts( const std::string &ip, int startPort, int endPort, int timeout )
{
    std::vector<int> openPorts;
    boost::asio::io_context io_context;

    for( int port = startPort; port <= endPort; ++port )
    {
        boost::asio::ip::tcp::resolver resolver( io_context );
        boost::asio::ip::tcp::resolver::query query( ip, std::to_string( port ) );
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );

        boost::asio::ip::tcp::socket socket( io_context );
        boost::system::error_code ec;
        socket.connect( *iterator, ec );

        if( !ec )
        {
            openPorts.push_back( port );
            LOG_INFO( "Open port found: %s:%d", ip.c_str(), port );
        }
        else
        {
            LOG_DEBUG( "Port closed: %s:%d", ip.c_str(), port );
        }

        socket.close();
    }

    return openPorts;
}

NetworkUtils::Pinger::Pinger( boost::asio::io_context &io_context, const std::string &ip, int port, int timeout )
    : m_Resolver( io_context ), m_Socket( io_context ), m_Deadline( io_context ), ip_( ip ), m_Port( port ), m_Timeout( timeout ), m_Alive( false )
{
    m_Deadline.expires_from_now( boost::posix_time::seconds( m_Timeout ) );
    StartPing();
    m_Deadline.async_wait( boost::bind( &Pinger::CheckDeadline, this ) );
}

bool NetworkUtils::Pinger::IsAlive() const
{
    return m_Alive;
}

void NetworkUtils::Pinger::StartPing()
{
    boost::asio::ip::tcp::resolver::query query( ip_, std::to_string( m_Port ) );
    m_Resolver.async_resolve( query, boost::bind( &Pinger::HandleResolve, this,
                                                  boost::asio::placeholders::error, boost::asio::placeholders::iterator ) );
    LOG_DEBUG( "Starting ping: %s:%d", ip_.c_str(), m_Port );
}

void NetworkUtils::Pinger::HandleResolve( const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator iterator )
{
    if( !ec )
    {
        boost::asio::async_connect( m_Socket, iterator, boost::bind( &Pinger::HandleConnect, this,
                                                                     boost::asio::placeholders::error ) );
    }
    else
    {
        LOG_WARN( "Resolve failed: %s", ec.message().c_str() );
    }
}

void NetworkUtils::Pinger::HandleConnect( const boost::system::error_code &ec )
{
    if( !ec )
    {
        m_Alive = true;
        LOG_INFO( "Ping successful: %s:%d", ip_.c_str(), m_Port );
        m_Deadline.cancel();
    }
    else
    {
        m_Alive = false;
        LOG_WARN( "Ping failed: %s:%d", ip_.c_str(), m_Port );
    }
}

void NetworkUtils::Pinger::CheckDeadline()
{
    if( m_Deadline.expires_at() <= boost::asio::deadline_timer::traits_type::now() )
    {
        m_Socket.close();
        m_Deadline.expires_at( boost::posix_time::pos_infin );
        LOG_DEBUG( "Ping deadline expired: %s:%d", ip_.c_str(), m_Port );
    }
    m_Deadline.async_wait( boost::bind( &Pinger::CheckDeadline, this ) );
}
