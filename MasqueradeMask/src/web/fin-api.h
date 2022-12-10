#pragma once
#include "ESPAsyncWebServer.h"
#include "../part/fins.h"

class FinApi
{
public:
    FinApi(Fins* _fins) {
        fins = _fins;
    }

    void attach(AsyncWebServer* server) {
        server->on("/api/fins", HTTP_GET, [&](AsyncWebServerRequest *request) {

            if (request->hasParam("angle", false)) {
                int angle = request->getParam("angle", false)->value().toInt();
                angle = min(255, angle);
                angle = max(0, angle);
                fins->setAllAngle(angle);
            }

            request->send(200, "application/json", "{}");
        });
    }

private:
    Fins* fins;
};
