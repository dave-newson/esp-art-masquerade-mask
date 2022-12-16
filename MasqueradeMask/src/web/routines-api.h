#pragma once
#include "ESPAsyncWebServer.h"
#include "../routines/routines.h"

class RoutinesApi
{
public:
    RoutinesApi(RoutineController* _routineController) {
        routineController = _routineController;
    }

    void attach(AsyncWebServer* server)
    {
        server->on("/api/routines", HTTP_GET, [&](AsyncWebServerRequest *request) {
            if (request->hasParam("next", false)) {
                routineController->next();
            }

            request->send(200, "application/json", "{}");
        });
    }

private:
    RoutineController* routineController;
};
