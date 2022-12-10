#pragma once
#include "ESPAsyncWebServer.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Masquerade Mask</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <h3>Masquerade Mask</h3>
  <br><br>
  <pre>
  /api/fins/
  /api/head-sides/
  /api/
  </pre>
</body></html>
)rawliteral";

class IndexPage
{
public:
    IndexPage() {}

    void attach(AsyncWebServer* server)
    {
        server->on("/", HTTP_GET, [&](AsyncWebServerRequest *request) {
            request->send_P(200, "text/html", index_html);
        });
    }

private:

};
