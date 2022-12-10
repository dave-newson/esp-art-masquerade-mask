#include "ESPAsyncWebServer.h"

class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){

    if (strcmp(request->host().c_str(), "masquerade.com")) {
        return true;
    }

    Serial.println(request->host().c_str());
    return false;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    request->send(200, "text/html", "<script>location.href='http://masquerade.com';</script>");
  }
};
