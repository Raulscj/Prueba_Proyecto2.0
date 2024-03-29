#include <WiFi.h>
#include <HTTPClient.h>
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
  //pinMode(4, OUTPUT);       // Bombillo
  //pinMode(15, OUTPUT);      // Motor
  pinMode(13, INPUT);       // Sensor de llama

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
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
    String html = "<html><head><style>";
    html += "body { background-color: #282c36; color: #dddddd; text-align: center; }";
    html += "h1 { color: #61dafb; }";
    html += "p { color: #ffee00; }";
    html += "form { display: inline-block; }";
    html += "</style></head><body>";
    html += "<h1>Sistema automatizado</h1>";
    html += "<p id='totalTime'>Tiempo Total: </p>";
    html += "<p id='remainingTime'>Tiempo Restante: </p>";
    html += "<p id='temperature'>Temperatura: </p>";
    html += "<p id='motor'>Estado del motor: </p>";
    html += "<p id='flauta'>Estado de las flautas: </p>";
    html += "<p id='sistema'>Estado del sistema: </p>";
    html += "<p id='tempMax'>Temperatura Maxima: </p>";
    html += "<p id='tempMin'>Temperatura Minima: </p>";
    html += "<form id='timeForm' action='/setTiempo' method='post'>";
    html += "Nuevo Tiempo: <input type='text' name='tiempo'><input type='submit' value='Actualizar'>";
    html += "Temperatura Maxima: <input type='number' name='tempMax'><input type='submit' value='Actualizar'>";
    html += "Temperatura Minima: <input type='number' name='tempMin'><input type='submit' value='Actualizar'>";
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
    html += "document.getElementById('motor').innerHTML = 'Estado del motor: ' + data.motor;";
    html += "document.getElementById('flauta').innerHTML = 'Estado de las flautas: ' + data.flauta;";
    html += "document.getElementById('sistema').innerHTML = 'Estado del sistema: ' + data.sistema;";
    html += "document.getElementById('tempMax').innerHTML = 'Temperatura Máxima: ' + data.tempMax + ' °C';";
    html += "document.getElementById('tempMin').innerHTML = 'Temperatura Mínima: ' + data.tempMin + ' °C';";
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
                          ", \"motor\":\"" + motor + "\"" +
                          ", \"flauta\":\"" + flauta + "\"" +
                          ", \"sistema\":\"" + sistema + "\"" +
                          ", \"tempMax\":" + String(temperaturaMaxima) +
                          ", \"tempMin\":" + String(temperaturaMinima) +
                          "}";
    server.send(200, "application/json", jsonResponse); });

  server.on("/setTiempo", HTTP_POST, []()
            {
    String nuevoTiempo = server.arg("tiempo");
    tiempoTotal = nuevoTiempo.toInt();
    tiempoRestante = tiempoTotal;
    String newtemperaturaMaxima = server.arg("tempMax");
    temperaturaMaxima = newtemperaturaMaxima.toFloat();
    String newtemperaturaMinima = server.arg("tempMin");
    temperaturaMinima = newtemperaturaMinima.toFloat();
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
    // digitalWrite(5, LOW);
    Serial.println("SYSTEMS ON");
    sistema = "on";
    delay(requestInterval);
    answer = getRequest(RelayMotorOn);
    Serial.println(answer);
    Serial.println("Encendiendo motor");
    motor = "on";
    moving();
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
    delay(1000);
  }
  else
  {
    Serial.println("SYSTEM OFF");
    sistema = "off";
    client.println("<script>");
    client.println("console.log('Systems off');");
    client.println("</script>");
    if (act)
    {
      Serial.println("Apagando motor");
      delay(requestInterval);
      answer = getRequest(RelayMotorOff);
      Serial.println(answer);
      motor = "off";
      alarm();
    }
  }
  delay(300);
}
