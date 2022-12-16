#pragma once
#include "ESPAsyncWebServer.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Masquerade Mask</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  body {
    background: black;
    color: white;
  }
  button {
    border: 2px solid yellow;
    padding: 1em;
    background: #000;
    color: yellow;
  }
  button:active {
    background: red;
  }
  </style>
  <script>
    function changeRoutine() {
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET", "/api/routines?next=true", true);
        xhttp.send();
    }
  </script>
  </head>
  <body>
  <h3>Dave's Masquerade Mask</h3>
  <br><br>
  <button onclick="changeRoutine()">Change Routine</button>
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
