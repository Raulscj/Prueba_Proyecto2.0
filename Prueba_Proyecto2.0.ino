#include <WiFi.h>
#include <ThingSpeak.h>
#include <max6675.h>
#include <WebServer.h>
/*#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>*/
#include "config.h"
#include "data.h"

void setup()
{
  pinMode(2, INPUT_PULLUP); // Pulsador
  pinMode(5, OUTPUT);       // Buzzer
  pinMode(4, OUTPUT);       // Bombillo
  pinMode(15, OUTPUT);      // Motor

  Serial.begin(115200);
  /*Wire.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);*/
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Conectando...");
    /*oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);
    oled.setTextSize(1);
    oled.print("Conectando...");
    oled.display();*/
  }
  Serial.println("Conexión exitosa");
  ThingSpeak.begin(client);
  Serial.println(WiFi.localIP());
  /*oled.clearDisplay();
  oled.setTextColor(WHITE);
  oled.setCursor(0, 0);
  oled.setTextSize(1);
  oled.print(WiFi.localIP());
  oled.display();*/
  delay(1000);

  server.on("/", HTTP_GET, []()
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
}

void loop()
{
  server.handleClient();
  if (digitalRead(2) == HIGH && tiempoRestante > 0)
  {
    
    Serial.println("SYSTEMS ON");
    getTemp();
    Serial.println("Temperatura:");
    /*oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setCursor(0, 0);
    oled.setTextSize(1);
    oled.print('Temperatura: ');
    oled.setCursor(36, 0);
    oled.setTextSize(1);
    oled.print(tempC);*/
    Serial.println(tempC);
    Serial.println("Tiempo ");
    Serial.println(tiempoRestante);
    /*oled.setCursor(0, 30);
    oled.setTextSize(1);
    oled.print('Tiempo restante: ');
    oled.setCursor(42, 30);
    oled.setTextSize(1);
    oled.print(tiempoRestante);
    oled.display();*/
    ThingSpeak.setField(3, tiempoRestante);
    ThingSpeak.writeFields(channelID, writeAPIKey);
    tiempoRestante --;
    server.handleClient();
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
  ThingSpeak.setField(1, tempC);
}
void alarm()
{
  digitalWrite(5, HIGH);
}
