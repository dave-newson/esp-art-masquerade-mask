#pragma once

#include "Wifi.h"
#include "DNSServer.h"

class WifiService
{
public:
    WifiService(DNSServer*);

    void begin();

    DNSServer* dnsServer;
};
