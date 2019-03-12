#ifndef __wifi_credentials_h
#define __wifi_credentials_h

const char* ssid = "YourSSID";
const char* password =  "YourPassword";

#define FIXED_IP

struct IpMac{
   String    Mac;
   IPAddress local_ip;
   IPAddress gateway;
   IPAddress subnet;
};

IPAddress defaultGateway = IPAddress(192, 168, 178, 1);
IPAddress defaultSubnet  = IPAddress(255, 255, 255, 0);

IpMac ipmacs[] = {
    {"AA:BB:CC:DD:EE:FF", IPAddress(192, 168, 178, 2), defaultGateway, defaultSubnet},
    {"00:11:22:33:44:55", IPAddress(192, 168, 178, 3), defaultGateway, defaultSubnet}
};

#endif