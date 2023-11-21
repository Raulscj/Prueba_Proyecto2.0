#include <WiFi.h>
#include <ThingSpeak.h>
#include <max6675.h>
#include <WebServer.h>
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
  pinMode(18, OUTPUT);      // Trigger
  pinMode(19, INPUT);       // ECO

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("Conectando...");
  }
  Serial.println("Conexión exitosa");
  ThingSpeak.begin(client);
  Serial.println(WiFi.localIP());
  delay(1000);

  server.on("/", HTTP_GET, []()
            {
    String html = "<html><head>";
    html += "<title>Tostadora de Maíz</title>";
    html += "<style>";
    html += "body {";
    html += "    background-color: gray;"; 
    html += "    text-align: center;"; 
    html += "}";
    html += "h1 {";
    html += "    color: #FFD700;"; 
    html += "}";
    html += "p {";
    html += "    color: #FFD700;"; 
    html += "    font-size: 18px;";
    html += "}";
    html += ".red-text {";
    html += "    color: red;"; 
    html += "}";
    html += "</style>";
    html += "</head><body>";
    html += "<h1>Tostadora de Maíz</h1>";
    html += "<p id='totalTime' class='red-text'>Tiempo Total: </p>";
    html += "<p id='remainingTime' class='red-text'>Tiempo Restante: </p>";
    html += "<p id='temperature' class='red-text'>Temperatura: </p>";
    html += "<p id='motor' class='red-text'>Estado del motor: </p>";
    html += "<p id='flautas' class='red-text'>Estado de las flautas: </p>";
    html += "<p id='system' class='red-text'>Estado del sistema: </p>";
    html += "<form id='timeForm' action='/setTiempo' method='post'>";
    html += "Nuevo Tiempo: <input type='text' name='tiempo'><input type='submit' value='Actualizar'>";
    html += "Temperatura maxima: <input type='number' name='tempmax'><input type='submit' value='Actualizar'>";
    html += "Temperatura minima: <input type='number' name='tempmin'><input type='submit' value='Actualizar'>";
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
    html += "document.getElementById('motor').innerHTML = 'Tiempo Total: ' + data.motor;";
    html += "document.getElementById('flautas').innerHTML = 'Tiempo Total: ' + data.flautas;";
    html += "document.getElementById('system').innerHTML = 'Tiempo Total: ' + data.system;";
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
                          ", \"temperature\":" + String(tempC) + 
                          ",\"motor\":" + String(motor) +
                          ",\"flautas\":" + String(flauta) +
                          ",\"system\":" + String(sistema) +
                          "}";
    server.send(200, "application/json", jsonResponse); });

  server.on("/setTiempo", HTTP_POST, []()
            {
    String nuevoTiempo = server.arg("tiempo");
    tiempoTotal = nuevoTiempo.toInt();
    tiempoRestante = tiempoTotal;
    String newtemperaturaMaxima = server.arg("tempmax");
    temperaturaMaxima = newtemperaturaMaxima.toInt();
    String newtemperaturaMinima = server.arg("tempmin");
    tiempoTotal = nuevoTiempo.toInt();
    temperaturaMinima = newtemperaturaMinima.toInt();
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
    sistema = "on";
    digitalWrite(15, HIGH);
    Serial.println("Encendiendo motor");
    motor = "on";
    fire();
    Serial.println("Encendiendo Gas");
    getTemp();
    Serial.println("Temperatura:");
    Serial.println(tempC);
    Serial.println("Tiempo ");
    Serial.println(tiempoRestante);
    ThingSpeak.setField(3, tiempoRestante);
    ThingSpeak.writeFields(channelID, writeAPIKey);
    tiempoRestante--;
  }
  else
  {
    Serial.println("SYSTEM OFF");
    sistema = "off";
    if (digitalRead(15) == HIGH || tiempoRestante == 0)
    {
      Serial.println("Apagando motor");
      motor = "off";
      digitalWrite(15, LOW);
      alarm();
    }
  }
  delay(300);
}
