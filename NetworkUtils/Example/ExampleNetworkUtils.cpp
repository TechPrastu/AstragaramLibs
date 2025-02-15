#include <iostream>
#include "NetworkUtils.h"
#include "Logger.h"

int main()
{
    Logger::SetPriority( LogLevel::TRACE );
    Logger::Info( "------------------- Example usage of GetNetworkInterfaces -----------------------" );
    // Example usage of GetNetworkInterfaces
    std::vector<NetworkUtils::NetworkInterface> interfaces = NetworkUtils::GetNetworkInterfaces();
    for( const auto &iface : interfaces )
    {
        Logger::Info( "---------------" );
        Logger::Info( "Name: %s", iface.name.c_str() );
        Logger::Info( "IPv4: %s", iface.ip4.c_str() );
        Logger::Info( "IPv6: %s", iface.ip6.c_str() );
        Logger::Info( "Subnet Mask: %s", iface.subnetMask.c_str() );
        Logger::Info( "Is Up: %s", ( iface.isUp ? "Yes" : "No" ) );
    }

    Logger::Info( "------------------- Example usage of CheckURLReachable ---------------------------" );
    // Example usage of CheckURLReachable
    std::string url = "http://www.example.com";
    bool isReachable = NetworkUtils::CheckURLReachable( url );
    Logger::Debug( "URL %s is %s", url.c_str(), ( isReachable ? "reachable" : "not reachable" ) );

    Logger::Info( "------------------- Example usage of GetNetworkInterfacesProto -------------------" );
    // Example usage of GetNetworkInterfacesProto
    std::vector<NetworkInterfaceProto> interfacesProto = NetworkUtils::GetNetworkInterfacesProto();
    for( const auto &ifaceProto : interfacesProto )
    {
        Logger::Info( "---------------" );
        Logger::Info( "Name: %s", ifaceProto.name().c_str() );
        Logger::Info( "IPv4: %s", ifaceProto.ip4().c_str() );
        Logger::Info( "IPv6: %s", ifaceProto.ip6().c_str() );
        Logger::Info( "Subnet Mask: %s", ifaceProto.subnetmask().c_str() );
        Logger::Info( "Is Up: %s", ( ifaceProto.isup() ? "Yes" : "No" ) );
    }

    Logger::Info( "------------------- Example usage of IsHostAlive ---------------------------------" );
    // Example usage of IsHostAlive
    std::string ip = "192.168.1.1";
    std::vector<int> ports = {80, 443};
    bool isAlive = NetworkUtils::IsHostAlive( ip, ports );
    Logger::Debug( "Host %s is %s", ip.c_str(), ( isAlive ? "alive" : "not alive" ) );

    return 0;
}
