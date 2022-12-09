#pragma once
#include "ESPAsyncWebServer.h"

class RoutinesApi
{
public:
    RoutinesApi() {}
    void attach(AsyncWebServer* server)
    {
        server->on("/api/routine", HTTP_POST, [&](AsyncWebServerRequest *request) {
            request->send(200, "application/json", "{}");
        });
    }

private:

};
