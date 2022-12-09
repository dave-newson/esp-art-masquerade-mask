#pragma once
#include "ESPAsyncWebServer.h"

class HeadSidesApi
{
public:
    HeadSidesApi() {}
    void attach(AsyncWebServer* server) {
        server->on("/api/head-sides", HTTP_POST, [&](AsyncWebServerRequest *request) {
            request->send(200, "application/json", "{}");
        });
    }

private:

};
