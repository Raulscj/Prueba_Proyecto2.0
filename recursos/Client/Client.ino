// Cliente
#include <Wifi.h>
#include <HTTPClient.h>

/*-------------CONFIG--------------------*/
const char *ssid = "ESP32-AP";
const char *password = "25041999";

// Url para hacer las peticiones
const char *esp32Server = "http://192.168.200.1:8080";

// Variables de programa

const byte btn = 13;
const byte led = 25;
bool isLedOn = false;
String answer;

void setup()
{
  Serial.begin(115200);
  // pin config
  pinMode(btn, INPUT_PULLDOWN);
  pinMode(led, OUTPUT);

  // nos conectamos a la red
  WiFi.begin(ssid, password);
  Serial.println("Conectando...");
}