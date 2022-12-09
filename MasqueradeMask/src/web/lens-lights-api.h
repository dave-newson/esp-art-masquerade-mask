#pragma once
#include "ESPAsyncWebServer.h"

class LensLightsApi
{
public:
    LensLightsApi() {}
    
    void attach(AsyncWebServer* server)
    {
        server->on("/api/lens-lights", HTTP_POST, [&](AsyncWebServerRequest *request) {
            request->send(200, "application/json", "{}");
        });
    }

private:

};
