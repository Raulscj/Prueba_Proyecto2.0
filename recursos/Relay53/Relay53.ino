#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "data.h"
#include "upload.h"
#include "controller.h

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

{
	server.send(404, "text/plain", "Not Found");
}