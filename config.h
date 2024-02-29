// Credenciales
const char *ssid = "ESP32-AP";
const char *password = "25041999";
unsigned long channelID = 2308886;
const char *writeAPIKey = "UQ571C2UZVDM7JGH";
WiFiClient client;
// Objetos
MAX6675 thermocouple(12, 14, 27);
WebServer server(8081);
