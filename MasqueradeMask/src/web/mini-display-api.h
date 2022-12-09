#pragma once
#include "ESPAsyncWebServer.h"

class MiniDisplayApi
{
public:
    MiniDisplayApi() {}
    void attach(AsyncWebServer* server)
    {
        server->on("/api/mini-display", HTTP_POST, [&](AsyncWebServerRequest *request) {
            request->send(200, "application/json", "{}");
        });
    }

private:

};
