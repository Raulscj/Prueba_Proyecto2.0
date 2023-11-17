#include <WiFi.h>
#include <ThingSpeak.h>
#include <max6675.h>
#include <WebServer.h>
#include "config.h"
#include "data.h"

void setup()
{
  pinMode(2, INPUT_PULLUP); // Pulsador
  pinMode(5, OUTPUT);       // Buzzer
  pinMode(4, OUTPUT);       // Bombillo
  pinMode(15, OUTPUT);      // Motor
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

  server.on("/", HTTP_GET,[]() {
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
}

void loop()
{
  if (digitalRead(2) == HIGH && tiempoRestante > 0)
  {
    Serial.println("SYSTEMS ON");
    server.handleClient();
    getTemp();

    Serial.println('Temperatura:');

    Serial.println(tempC);

    Serial.println('Tiempo');
    ThingSpeak.setField(3, tiempoRestante);
    ThingSpeak.writeFields(channelID, writeAPIKey);
  }
  else
  {
    Serial.println("SYSTEM OFF");
  }
  delay(300);
}
void getTemp()
{
  tempC = thermocouple.readCelsius();
  ThingSpeak.setField (1,tempC);
}
