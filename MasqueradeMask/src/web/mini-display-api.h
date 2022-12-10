#pragma once
#include "ESPAsyncWebServer.h"
#include "../part/display-mini.h"

class MiniDisplayApi
{
public:
    MiniDisplayApi(DisplayMini* _displayMini) {
        displayMini = _displayMini;
    }

    void attach(AsyncWebServer* server)
    {
        server->on("/api/mini-display", HTTP_GET, [&](AsyncWebServerRequest *request) {
            if (request->hasParam("message", false)) {
                char message[16];
                request->getParam("message", false)->value().toCharArray(message, 16);
                displayMini->getDriver()->drawStr(0, 7, message);
            }

            request->send(200, "application/json", "{}");
        });
    }

private:
    DisplayMini* displayMini;

};
