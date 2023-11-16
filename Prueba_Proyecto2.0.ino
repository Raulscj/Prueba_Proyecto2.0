#include <WiFi.h>
#include <ThingSpeak.h>
#include <max6675.h>
#include <WebServer.h>

// Conexiones
const char *ssid = "radscj";
const char *password = "26962904";
unsigned long channelID = 2308886;
const char *writeAPIKey = "UQ571C2UZVDM7JGH";
WiFiClient client;
MAX6675 thermocouple(12, 14, 27);
WebServer server(80);

// Variables globales
unsigned long tiempoInicio;
const char *titleTemp = "Temp: ";
const char *titleTime = "TimeT: ";
float tempC; // Definimos las variables globales aquí
float timeT;

void setup()
{
  pinMode(2, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Proyecto");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Conectando...");
  }
  Serial.println("Conexión exitosa");
  ThingSpeak.begin(client);
  Serial.println(WiFi.localIP());
  server.on("/", inicio);
  server.begin();
  Serial.println("Servidor HTTP iniciado");
  tiempoInicio = millis();
}

void loop()
{
  if (digitalRead(2) == HIGH)
  {
    Serial.println("SYSTEMS ON");
    server.handleClient();
    getTemp();

    Serial.println(titleTemp);

    Serial.println(tempC);

    Serial.println(titleTime);

    Serial.println((millis() - tiempoInicio) / 1000);
    timeT = ((millis() - tiempoInicio) / 1000);
    ThingSpeak.setField(3, timeT);
    ThingSpeak.writeFields(channelID, writeAPIKey);
    // delay(300);
  }
  else
  {
    Serial.println("SYSTEM OFF");
  }
  delay(300);
}
void inicio()
{
  if (server.hasArg("led1") && server.arg("led") == "on") // hasArg si en el codigo esta la variable led1 es true, y arg evalua su valor
  {
    Serial.print("Encendido");
  }
  else if (server.hasArg("led1") && server.arg("led") == "off")
  {
    Serial.print("Apagado");
  }
  // Pagina HTML
  String html = "<html><body>";
  html += "<head><meta name=\"viewport\" content=\"width = device-width, initial-scale = 1.0, user-scalable = no\">\n";
  html += "</head>";
  html += "<title> Prueba de conexion</title>\n";
  html += "<h1>Sistema de control de tostadora de maiz</h1>";
  // html += "<p>Tiempo transcurrido: " String(timeT) + "</p>";
  html += "<form method='get'>";
  html += "<button type='submit' name='led' value='on'>Encender</button>";
  html += "<button type='submit' name='led' value='off'>Apagar</button>";
  html += "</form>";
  html += "</body></html>";

  // Enviar la pagina
  server.send(200, "text/html", html);
  delay(300);
}
void getTemp()
{
  tempC = thermocouple.readCelsius();
  ThingSpeak.setField(1, tempC);
}
