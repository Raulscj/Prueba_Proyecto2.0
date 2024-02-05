#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "data.h"
#include "upload.h"
ESP8266WebServer server(80);

String State = "OFF";

void setup(void) {
  pinMode(Relay_PIN, OUTPUT);
  digitalWrite(Relay_PIN, LOW);
  
  Serial.begin(115200);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(WIFI_SSID, WIFI_Password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  //Vistas
  server.on("/", handleRoot);
  server.on("/subir", HTTP_GET, PaginaSimple);
  server.on("/actualizar", HTTP_POST, ActualizarPaso1, ActualizarPaso2);
  server.begin();
  Serial.println("HTTP server started");
  MDNS.addService("http", "tcp", 80);
    Serial.printf("Listo!\nAbre http://%s.local en navegador\n", ("esp8266"));
    Serial.print("o en la IP: ");
    Serial.println(WiFi.localIP());
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  delay(2);
}

void handleRoot() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    Serial.println(server.arg(0));
    Serial.println(server.arg(1));

    if(server.arg(0) == Relay_Password) {
       if(server.arg(1) == "ON") {
           State = "ON";
           server.send(200, "text/plain", State);
           digitalWrite(Relay_PIN, HIGH);
       }
       else if(server.arg(1) == "OFF") {
           State = "OFF";
           server.send(200, "text/plain", State);
           digitalWrite(Relay_PIN, LOW);
       }
       else if(server.arg(1) == "STATE") {
           server.send(200, "text/plain", State);
       }
       else {
           server.send(200, "text/plain", "UNKNOWN_COMMAND");
       }
    }
    else {
       server.send(401, "text/plain", "AUTH_FAIL");
    }
  }
}
