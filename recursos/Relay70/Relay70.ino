#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "data.h"
#include "upload.h"

ESP8266WebServer server(8082);
void setup()
{
  Serial.begin(115200);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
   WiFi.config(ip, gateway, subnet);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  if (WiFi.waitForConnectResult() == WL_CONNECTED)
  {

    // Definir rutas del servidor
  server.on("/RELAY=ON", HTTP_GET, []() {
    Serial.println("RELAY=ON");
    digitalWrite(RELAY, LOW);
    value = LOW;
    server.send(200, "text/plain", "Relé encendido");
  });
  server.on("/RELAY=OFF", HTTP_GET, []() {
    Serial.println("RELAY=OFF");
    digitalWrite(RELAY, HIGH);
    value = HIGH;
    server.send(200, "text/plain", "Relé apagado");
  });
    server.on("/actualizar", HTTP_POST, ActualizarPaso1, ActualizarPaso2);
    server.begin();

    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("Error en Wifi");
  }
}
void loop()
{
  server.handleClient();

  // Check if a client has connected
  WiFiClient client = server.client();
  if (!client)
  {
    return;
  }
  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  // Match the request
  
  if (request.indexOf("/RELAY=ON") != -1)
  {
    Serial.println("RELAY=ON");
    client.println("<script>");
    client.println("console.log('Relay ON');");
    client.println("</script>");
    digitalWrite(RELAY, LOW);
    value = LOW;
  }
  if (request.indexOf("/RELAY=OFF") != -1)
  {
    Serial.println("RELAY=OFF");
    client.println("<script>");
    client.println("console.log('Relay OFF');");
    client.println("</script>");
    digitalWrite(RELAY, HIGH);
    value = HIGH;
  }

  // Display the HTML web page
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  this is a must
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<title>ESP8266 RELAY Control</title>");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #77878A;}</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1 align = center>ESP01 RELAY CONTROL</h1>");
  client.print("<h2 align = center>RELAY STATUS: ");

  if (value == HIGH)
  {
    client.print("OFF");
  }
  else
  {
    client.print("ON");
  }
  client.println("</h2>");
  client.println("<br>");
  client.println("<a href=\"/RELAY=ON\"><button class=\"button\">TURN ON</button></a>");
  client.println("<a href=\"/RELAY=OFF\"><button class=\"button button2\">TURN OFF</button></a><br>");
  client.println("<h2> UPDATE </h2>");
  client.println("<form method='POST' action='/actualizar' enctype='multipart/form-data'><input type='file' name='update' class=\"button\"><br><input type='submit' value='actualizar' class=\"button button2\"></form>");
  client.println("</body>");
  client.println("</html>");
  client.println("<script>");
  client.println("console.log('Este es un mensaje desde el código JavaScript en la página HTML');");
  client.println("</script>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
