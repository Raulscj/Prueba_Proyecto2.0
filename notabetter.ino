#include <WiFi.h>
#include <max6675.h>
#include <WebServer.h>
#include "config.h"
#include "data.h"

// Definir pines
const int motorPin = 2;
const int bombilloPin = 4;
const int pulsadorPin = 5;
const int ultrasonidoPin = 6;
const int buzzerPin = 15;

// Crear objeto WebServer
WebServer server(80);

// Variables globales
unsigned long tiempoInicio;
int tiempoTotal = 0;
int tiempoRestante = 0;
int temperaturaMaxima = 200;
float tempC;
float timeT;

void setup()
{
  pinMode(motorPin, OUTPUT);
  pinMode(bombilloPin, OUTPUT);
  pinMode(pulsadorPin, INPUT);
  pinMode(ultrasonidoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

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

  server.on("/", HTTP_GET, [](WebServer::ConnectionType type, char *url_tail, bool tail_complete)
            {
    String html = "<html><body>";
    html += "<h1>Tostadora de Maíz</h1>";
    html += "<p>Tiempo Total: " + String(tiempoTotal) + " segundos</p>";
    html += "<p>Tiempo Restante: " + String(tiempoRestante) + " segundos</p>";
    html += "<p>Temperatura: " + String(tempC) + " °C</p>";
    html += "<form action='/setTiempo' method='post'>";
    html += "Nuevo Tiempo: <input type='text' name='tiempo'><input type='submit' value='Actualizar'>";
    html += "</form></body></html>";
    server.send(200, "text/html", html); });

  server.on("/setTiempo", HTTP_POST, []()
            {
    String nuevoTiempo = server.arg("tiempo");
    tiempoTotal = nuevoTiempo.toInt();
    tiempoRestante = tiempoTotal;
    server.sendHeader("Location", "/");
    server.send(302); });

  server.begin();
  Serial.println("Servidor HTTP iniciado");
  tiempoInicio = millis();
}

void loop()
{
  if (digitalRead(2) == HIGH)
  {
    server.handleClient();
    getTemp();

    if (tiempoRestante > 0)
    {
      tiempoRestante--;

      digitalWrite(motorPin, HIGH);
      digitalWrite(bombilloPin, HIGH);

      if (thermo.readCelsius() > temperaturaMaxima)
      {
        apagarTostadora();
        activarBuzzer();
      }

      if (digitalRead(ultrasonidoPin) == HIGH)
      {
        apagarBuzzer();
        digitalWrite(bombilloPin, LOW);
      }
    }
    else
    {
      apagarTostadora();
    }
    delay(300);
  }
  else
  {
    Serial.println("SYSTEM OFF");
  }
}

void getTemp()
{
  tempC = thermo.readCelsius();
  ThingSpeak.setField(1, tempC);
  timeT = ((millis() - tiempoInicio) / 1000);
  ThingSpeak.setField(3, timeT);
  ThingSpeak.writeFields(channelID, writeAPIKey);
}

void apagarTostadora()
{
  digitalWrite(motorPin, LOW);
  digitalWrite(bombilloPin, LOW);
}

void activarBuzzer()
{
  digitalWrite(buzzerPin, HIGH);
}

void apagarBuzzer()
{
  digitalWrite(buzzerPin, LOW);
}
