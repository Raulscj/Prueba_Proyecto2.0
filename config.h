// Credenciales
const char *ssid = "ESP32-AP";
const char *password = "25041999";
const IPAddress ip(192, 168, 200, 80); // Dirección IP estática para esta placa
const IPAddress gateway(192, 168, 200, 1); // Dirección IP del gateway
const IPAddress subnet(255, 255, 255, 0); // Máscara de subred

unsigned long channelID = 2308886;
const char *writeAPIKey = "UQ571C2UZVDM7JGH";
WiFiClient client;
// Objetos
MAX6675 thermocouple(12, 14, 27);
WebServer server(80);
