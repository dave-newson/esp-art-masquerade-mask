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
        server->on("/api/routine", HTTP_GET, [&](AsyncWebServerRequest *request) {
            if (request->hasParam("routine", false)) {
                char routine[16];
                request->getParam("routine", false)->value().toCharArray(routine, 16);
                routineController->setRoutine(routine);
            }

            request->send(200, "application/json", "{}");
        });
    }

private:
    RoutineController* routineController;
};
