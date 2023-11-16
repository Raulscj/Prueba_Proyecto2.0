// Conexiones
const char *ssid = "radscj";
const char *password = "26962904";
unsigned long channelID = 2308886;
const char *writeAPIKey = "UQ571C2UZVDM7JGH";
WiFiClient client;
MAX6675 thermocouple(12, 14, 27);
WebServer server(80);