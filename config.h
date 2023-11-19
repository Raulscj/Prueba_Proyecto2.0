// Credenciales
const char *ssid = "radscj";
const char *password = "26962904";
unsigned long channelID = 2308886;
const char *writeAPIKey = "UQ571C2UZVDM7JGH";
WiFiClient client;
//Objetos
MAX6675 thermocouple(12, 14, 27);
Adafruit_SSD1306 oled(128, 64, &Wire, 4);
WebServer server(80);