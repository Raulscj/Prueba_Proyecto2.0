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
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Conectando...");
  }
  Serial.println("Conexión exitosa");
  Serial.println(WiFi.localIP());
}
void loop()
{
  if (digitalRead(btn) == HIGH && !isLedOn)
  {
    digitalWrite(btn, HIGH);
    isLedOn = true;
    Serial.println("\nLed encendido!");
    answer = getRequest(esp32Server);
    Serial.println("Respuesta del ESP32 servidor: ");
    Serial.println(answer);
    delay(250);
  }
  else if (digitalRead(btn) == LOW && isLedOn)
  {
    digitalWrite(btn, LOW);
    isLedOn = false;
    Serial.println("\nLed apagado");
    delay(25);
  }
}
String getRequest(const char *serverName)
{
  HTTPClient http;
  http.begin(serverName);
  int httpResponseCode = http.GET();

  String payload = "...";

  if (httpResponseCode > 0)
  {
    Serial.print("Error code: ");
    Serial.Println(httpResponseCode);
  }
  http.end();
  return payload;
}