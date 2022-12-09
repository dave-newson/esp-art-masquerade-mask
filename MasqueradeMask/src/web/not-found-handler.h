#pragma once
#include "ESPAsyncWebServer.h"

class NotFoundHandler
{
public:
    NotFoundHandler() {};
    void attach(AsyncWebServer* server) {
      server->onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/html", "<h1>Not found</h1>");
      });
    }

private:

};
