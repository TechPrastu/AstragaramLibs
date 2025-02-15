#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <thread>
#include <google/protobuf/message.h> // Adjust the include path based on your protobuf setup
#include "NetworkInterface.pb.h" // Include the generated protobuf header
#include "Logger.h" // Include custom Logger

class NetworkUtils
{
public:
    struct NetworkInterface
    {
        std::string name;
        std::string ip4;
        std::string ip6;
        std::string subnetMask;
        bool isUp;
        // Add other fields as needed
    };

    static std::vector<NetworkInterface> GetNetworkInterfaces();
    static bool IsHostAlive( const std::string &ip, const std::vector<int> &ports, int timeout = 10 );
    static bool IsHostAliveICMP( const std::string &ip );
    static bool CheckURLReachable( const std::string &url, int timeout = 10 );
    static std::vector<NetworkInterfaceProto> GetNetworkInterfacesProto();
    static std::vector<std::string> FindAvailableDevices( const std::string &subnet, int cidr, const std::vector<int> &ports );
    static std::vector<int> FindOpenPorts( const std::string &ip, int startPort, int endPort, int timeout );

private:
    static std::string GetSubnet( const std::string &ip, const std::string &mask );
    static int GetCIDR( const std::string &mask );
    static unsigned short InCksum( unsigned short *addr, int len );

    class Pinger
    {
    public:
        Pinger( boost::asio::io_context &io_context, const std::string &ip, int port, int timeout );
        bool IsAlive() const;

    private:
        void StartPing();
        void HandleResolve( const boost::system::error_code &ec, boost::asio::ip::tcp::resolver::iterator iterator );
        void HandleConnect( const boost::system::error_code &ec );
        void CheckDeadline();

        boost::asio::ip::tcp::resolver m_Resolver;
        boost::asio::ip::tcp::socket m_Socket;
        boost::asio::deadline_timer m_Deadline;
        std::string ip_;
        int m_Port;
        int m_Timeout;
        bool m_Alive;
    };
};

#endif // NETWORKUTILS_H
