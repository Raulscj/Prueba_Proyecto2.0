// Credenciales
const char *ssid = "radscj";
const char *password = "26962904";
unsigned long channelID = 2308886;
const char *writeAPIKey = "UQ571C2UZVDM7JGH";
WiFiClient client;
//Punto de anclaje
const char* apSSID = "ESP32-AP";
const char* apPassword = "25041999";
IPAddress apIP(192, 168, 200, 1); // Dirección IP estática para el punto de acceso

// Objetos
MAX6675 thermocouple(12, 14, 27);
WebServer server(8080);
