#include "wifi-service.h"

WifiService::WifiService(DNSServer* _dnsServer)
{
    dnsServer = _dnsServer;
}

void WifiService::begin()
{
  IPAddress apIP(8,8,4,4); // The default android DNS

  WiFi.mode(WIFI_AP); 
  WiFi.softAP("Masquerade");
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));


  // Redirect all traffic to this AP's IP
  dnsServer->start(53, "*", WiFi.softAPIP());
}
