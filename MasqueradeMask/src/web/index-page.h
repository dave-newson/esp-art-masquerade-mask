#pragma once
#include "ESPAsyncWebServer.h"

class IndexPage
{
public:
    IndexPage() {}

    void attach(AsyncWebServer* server)
    {
        server->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
            request->send(200, "text/html", "<h1>Hello World</h1>");
        });
    }


private:

};
