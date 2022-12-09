#pragma once
#include "ESPAsyncWebServer.h"

class FinApi
{
public:
    FinApi() {}

    void attach(AsyncWebServer* server) {
            server->on("/api/fins", HTTP_POST, [&](AsyncWebServerRequest *request) {
            request->send(200, "application/json", "{}");
        });
    }

private:

};
