#include <ESPAsyncWebServer.h>
#include <Max6675.h>

// Definir pines
const int motorPin = 2;       // Pin para el control del motor
const int bombilloPin = 4;    // Pin para el control del bombillo
const int pulsadorPin = 5;    // Pin para el pulsador de apagado de emergencia
const int ultrasonidoPin = 6; // Pin para el sensor de ultrasonido

// Definir pines para el sensor de temperatura Max6675
const int thermoDO = 12;
const int thermoCS = 13;
const int thermoCLK = 14;

// Definir pines para el buzzer
const int buzzerPin = 15;

// Definir tiempos y temperaturas
int tiempoTotal = 0;         // Tiempo total de tostado en segundos
int tiempoRestante = 0;      // Tiempo restante de tostado en segundos
int temperaturaMaxima = 200; // Temperatura máxima permitida

// Crear objetos
AsyncWebServer asyncServer(80);
Max6675 thermo(thermoCLK, thermoCS, thermoDO);

void setup()
{
  // Inicializar pines
  pinMode(motorPin, OUTPUT);
  pinMode(bombilloPin, OUTPUT);
  pinMode(pulsadorPin, INPUT);
  pinMode(ultrasonidoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Configurar servidor web
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String html = "<html><body>";
    html += "<h1>Tostadora de Maíz</h1>";
    html += "<p>Tiempo Total: " + String(tiempoTotal) + " segundos</p>";
    html += "<p>Tiempo Restante: " + String(tiempoRestante) + " segundos</p>";
    html += "<p>Temperatura: " + String(thermo.readCelsius()) + " °C</p>";
    html += "<form action='/setTiempo' method='post'>";
    html += "Nuevo Tiempo: <input type='text' name='tiempo'><input type='submit' value='Actualizar'>";
    html += "</form></body></html>";
    request->send(200, "text/html", html); });

  server.on("/setTiempo", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    String nuevoTiempo = request->arg("tiempo");
    tiempoTotal = nuevoTiempo.toInt();
    tiempoRestante = tiempoTotal;
    request->redirect("/"); });

  // Iniciar servidor
  server.begin();
}

void loop()
{
  // Actualizar el tiempo restante
  if (tiempoRestante > 0)
  {
    tiempoRestante--;

    // Encender el motor y el bombillo
    digitalWrite(motorPin, HIGH);
    digitalWrite(bombilloPin, HIGH);

    // Supervisar la temperatura
    if (thermo.readCelsius() > temperaturaMaxima)
    {
      apagarTostadora();
      activarBuzzer();
    }

    // Supervisar el sensor de ultrasonido
    if (digitalRead(ultrasonidoPin) == HIGH)
    {
      apagarBuzzer();
      digitalWrite(bombilloPin, LOW);
    }
  }
  else
  {
    // Apagar el motor y el bombillo cuando el tiempo ha terminado
    apagarTostadora();
  }

  // Supervisar el pulsador de apagado de emergencia
  if (digitalRead(pulsadorPin) == HIGH)
  {
    apagarTostadora();
    activarBuzzer();
  }

  // Agregar otras comprobaciones y lógica según sea necesario
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
