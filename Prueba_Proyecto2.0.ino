#include <WiFi.h>
#include <ThingSpeak.h>
#include <max6675.h>
#include <WebServer.h>
/*#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>*/
#include "config.h"
#include "data.h"
#include "controllers.h"

void setup()
{
  pinMode(2, INPUT_PULLUP); // Pulsador
  pinMode(5, OUTPUT);       // Buzzer
  pinMode(4, OUTPUT);       // Bombillo
  pinMode(15, OUTPUT);      // Motor
  pinMode(13, INPUT);       // Sensor de llama
  pinMode(18, OUTPUT); //Trigger
  pinMode(19, INPUT); //ECO

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
    html += "<p id='totalTime'>Tiempo Total: </p>";
    html += "<p id='remainingTime'>Tiempo Restante: </p>";
    html += "<p id='temperature'>Temperatura: </p>";
    html += "<form id='timeForm' action='/setTiempo' method='post'>";
    html += "Nuevo Tiempo: <input type='text' name='tiempo'><input type='submit' value='Actualizar'>";
    html += "</form>";
    html += "<script>";
    html += "function updateData() {";
    html += "var xhr = new XMLHttpRequest();";
    html += "xhr.onreadystatechange = function() {";
    html += "if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "var data = JSON.parse(xhr.responseText);";
    html += "document.getElementById('totalTime').innerHTML = 'Tiempo Total: ' + data.totalTime + ' segundos';";
    html += "document.getElementById('remainingTime').innerHTML = 'Tiempo Restante: ' + data.remainingTime + ' segundos';";
    html += "document.getElementById('temperature').innerHTML = 'Temperatura: ' + data.temperature + ' °C';";
    html += "}";
    html += "};";
    html += "xhr.open('GET', '/data', true);";
    html += "xhr.send();";
    html += "}";
    html += "setInterval(updateData, 1000);"; // Actualiza cada segundo
    html += "</script></body></html>";

    server.send(200, "text/html", html); });

  server.on("/data", HTTP_GET, []()
            {
    String jsonResponse = "{\"totalTime\":" + String(tiempoTotal) +
                          ", \"remainingTime\":" + String(tiempoRestante) +
                          ", \"temperature\":" + String(tempC) + "}";
    server.send(200, "application/json", jsonResponse); });

  server.on("/setTiempo", HTTP_POST, []()
            {
    String nuevoTiempo = server.arg("tiempo");
    tiempoTotal = nuevoTiempo.toInt();
    tiempoRestante = tiempoTotal;
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "Redirecting"); });

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop()
{
  server.handleClient();
  if (digitalRead(2) == HIGH && tiempoRestante > 0)
  {
    Serial.println("SYSTEMS ON");
    digitalWrite(15, HIGH);
    Serial.println("Encendiendo motor");
    fire();
    Serial.println("Encendiendo Gas");
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
    tiempoRestante--;
  }
  else
  {
    Serial.println("SYSTEM OFF");
    if (digitalRead(15) == HIGH)
    {
      Serial.println("Apagando motor");
      digitalWrite(15, LOW);
      alarm();
    }
  }
  delay(300);
}
