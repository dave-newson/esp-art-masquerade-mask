#pragma once
#include "ESPAsyncWebServer.h"

class EyeballApi
{
public:
    EyeballApi() {};

    void attach(AsyncWebServer* server) {
            server->on("/api/eyeball", HTTP_GET, [&](AsyncWebServerRequest *request) {
            request->send(200, "application/json", "{}");
        });
    }

private:

};
