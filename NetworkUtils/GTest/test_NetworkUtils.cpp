#include "NetworkUtils.h"
#include <gtest/gtest.h>

// Test for getting network interfaces
TEST( NetworkUtilsTest, GetNetworkInterfaces )
{
    std::vector<NetworkUtils::NetworkInterface> interfaces = NetworkUtils::GetNetworkInterfaces();
    ASSERT_FALSE( interfaces.empty() ) << "Network interfaces should not be empty";
}

// Test for ICMP host alive check
TEST( NetworkUtilsTest, IsHostAliveICMP )
{
    std::string ip = "127.0.0.1"; // localhost should always be alive
    bool result = NetworkUtils::IsHostAliveICMP( ip );
    ASSERT_TRUE( result ) << "Localhost (127.0.0.1) should be alive";
}

// Test for checking URL reachability
TEST( NetworkUtilsTest, CheckURLReachable )
{
    std::string url = "www.google.com";
    bool result = NetworkUtils::CheckURLReachable( url );
    ASSERT_TRUE( result ) << "Google URL (www.google.com) should be reachable";
}

// Test for getting network interfaces as protobuf
TEST( NetworkUtilsTest, GetNetworkInterfacesProto )
{
    std::vector<NetworkInterfaceProto> protoInterfaces = NetworkUtils::GetNetworkInterfacesProto();
    ASSERT_FALSE( protoInterfaces.empty() ) << "Protobuf network interfaces should not be empty";
}

// Test for finding available devices on the network
TEST( NetworkUtilsTest, FindAvailableDevices )
{
    std::string subnet = "192.168.1";
    int cidr = 24;
    std::vector<int> ports = {80, 443, 22};
    std::vector<std::string> devices = NetworkUtils::FindAvailableDevices( subnet, cidr, ports );
    ASSERT_FALSE( devices.empty() ) << "There should be at least one active device in the subnet";
}

// Test for finding open ports on a given IP address
TEST( NetworkUtilsTest, FindOpenPorts )
{
    std::string ip = "127.0.0.1"; // Use localhost for testing
    int startPort = 8000;
    int endPort = 8100;
    int timeout = 1; // 1 second timeout

    std::vector<int> openPorts = NetworkUtils::FindOpenPorts( ip, startPort, endPort, timeout );

    // Validate the results
    for( int port : openPorts )
    {
        std::cout << "Open port found: " << port << std::endl;
    }

    // Example assertion, adapt as necessary for your test environment
    EXPECT_GT( openPorts.size(), 0 ) << "No open ports found in the specified range.";
}

int main( int argc, char **argv )
{
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
