#pragma once
#include "ESPAsyncWebServer.h"

class MiscApi
{
public:
    MiscApi() {}
    void attach(AsyncWebServer* server)
    {
        server->on("/api/misc", HTTP_POST, [&](AsyncWebServerRequest *request) {
            request->send(200, "application/json", "{}");
        });
    }

private:

};
