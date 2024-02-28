//SERVIDOR PRINCIPAL
#include <Wifi.h>
#include <WebServer.h>

const char* apSSID = "ESP32-AP";
const char* apPassword = "25041999";
IPAddress apIP(192, 168, 200, 1); 
WebServer server(8080);
void setup() {
  Serial.begin(115200);
  delay(50);
  WiFi.softAP(apSSID, apPassword);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  Serial.print("Dirección IP del AP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleConnectionRoot);
  server.on("/device2", handleD1);
  server.on("/device1", handleD2);
  server.on("/device3", handleD3);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Empezo lo good");
  Serial.println("\nDispositivos contactados:")
}

void loop() {
  server.handleClient();
}
String device = "";
String answer = "";
void setAnswer(){
  answer = "<!DOCTYPE html>\
            <html>\
            <body>\
            <hola \"" + device + "\" !</h1>
            </body>\
            </html>";
}
void handleConnectionRoot(){
  server.send(200, "text/html", "Hola desconocido!");
}

