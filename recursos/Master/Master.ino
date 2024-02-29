// SERVIDOR PRINCIPAL
#include <WiFi.h>
#include <WebServer.h>

const char *apSSID = "ESP32-AP";
const char *apPassword = "25041999";
/*IPAddress ip(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);*/
IPAddress ip(192, 168, 200, 1);
IPAddress gateway(192, 168, 200, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);
void setup()
{
	Serial.begin(115200);
	delay(500);
	WiFi.softAP(apSSID, apPassword);
	WiFi.softAPConfig(ip, gateway, subnet);
	Serial.print("Dirección IP del AP: ");
	Serial.println(WiFi.softAPIP());

	server.on("/", handleConnectionRoot);
	server.onNotFound(handleNotFound);

	server.begin();
	Serial.println("Empezo lo good");
	Serial.println("\nDispositivos contactados:");
}

void loop()
{
	server.handleClient();
}
void handleConnectionRoot()
{
	server.send(200, "text/plain", "Hola desconocido!");
}
void handleNotFound()
{
	server.send(404, "text/plain", "Not Found");
}