#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "data.h"
#include "upload.h"


void setup() 
{
  Serial.begin(115200);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Definir rutas del servidor
  server.on("/",handleConnectionRoot);
  server.on("/RELAY=ON", handleRelayOn);
  server.on("/RELAY=OFF", handleRelayOff);
  server.onNotFound(handleNotFound);
  server.on("/actualizar", HTTP_POST, ActualizarPaso1, ActualizarPaso2);
  server.begin();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}
void loop()
{
  server.handleClient();
  delay(1);
}
String device = "";
String answer = "";
String home = "";
void setAnswer()
{
  answer = "<!DOCTYPE html>\
            <html>\
            <body>\
            <h1> Relay \"" +
            device + "\" !</h1>\
            </body>\
            </html> ";
}
void handleRoot()
{
  String request = server.arg(0); // Obtener la solicitud del cliente
  Serial.println(request);

  if (request.indexOf("/RELAY=ON") != -1)
  {
    Serial.println("RELAY=ON");
    digitalWrite(RELAY, LOW);
    value = LOW;
  }
  else if (request.indexOf("/RELAY=OFF") != -1)
  {
    Serial.println("RELAY=OFF");
    digitalWrite(RELAY, HIGH);
    value = HIGH;
  }

  server.send(200, "text/html", "<script>console.log('Response sent');</script>");
}
void handleConnectionRoot(){
  String relayStatus = (value == HIGH) ? "OFF" : "ON"; // Concatenar el estado del relé
  
  String home = "<!DOCTYPE html>\n<html>\n  <head>\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\n    <title>ESP8266 RELAY Control</title>\n    <style>\n      html {\n        font-family: Helvetica;\n        display: inline-block;\n        margin: 0px auto;\n        text-align: center;\n      }\n\n      .button {\n        background-color: #195b6a;\n        border: none;\n        color: white;\n        padding: 16px 40px;\n        text-decoration: none;\n        font-size: 30px;\n        margin: 2px;\n        cursor: pointer;\n      }\n      .button2 {\n        background-color: #77878a;\n      }\n    </style>\n  </head>\n  <body>\n    <h1 align=\"center\">ESP01 RELAY CONTROL</h1>\n    <h2 align=\"center\">RELAY STATUS: " + relayStatus + "</h2>\n    <br />\n    <a href=\"/RELAY=ON\"><button class=\"button\">TURN ON</button></a>\n    <a href=\"/RELAY=OFF\"><button class=\"button button2\">TURN OFF</button></a>\n    <br />\n    <h2>UPDATE</h2>\n    <form method=\"POST\" action=\"/actualizar\" enctype=\"multipart/form-data\">\n      <input type=\"file\" name=\"update\" class=\"button\" /><br />\n      <input type=\"submit\" value=\"actualizar\" class=\"button button2\" />\n    </form>\n    <script>\n      console.log(\n        \"Este es un mensaje desde el código JavaScript en la página HTML\"\n      );\n    </script>\n  </body>\n</html>\n";

  server.send(200, "text/html", home);
}

void handleRelayOn(){
  device = "Encendido";
  Serial.println("RELAY=ON");
  digitalWrite(RELAY, LOW);
  value = LOW;
  setAnswer();
  server.send(200, "text/html", answer);
}
void handleRelayOff(){
  device = "Apagado";
  Serial.println("RELAY=OFF");
  digitalWrite(RELAY, HIGH);
  value = HIGH;
  setAnswer();
  server.send(200, "text/html", answer);
}
void handleNotFound()
{
  server.send(404, "text/plain", "Not Found");
}
