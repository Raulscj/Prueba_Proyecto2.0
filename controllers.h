
void getTemp()
{
  tempC = thermocouple.readCelsius();
  ThingSpeak.setField(1, tempC);
}
void alarm()
{
  static unsigned long startTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - startTime < 2500)
  {
    if (digitalRead(5) == LOW)
    {
      digitalWrite(5, HIGH);
      return;
    }
  }
  else
  {
    digitalWrite(5, LOW);
    startTime = currentTime;
  }
}
void fire()
{
  // Cuando se apaga
  if (tempC > temperaturaMaxima)
  {
    Serial.println("Temperatura excedida");
    flauta = "off";
  }
  if (digitalRead(13) == LOW || !act)
  {
    Serial.println("Apagando flautas");
    digitalWrite(4, LOW);
    flauta = "off";
    alarm();
  }
  // Cuando enciende
  if (tempC < temperaturaMinima)
  {
    Serial.println("Encendiendo flautas");
    digitalWrite(4, HIGH);
    flauta = "on";
    digitalWrite(5, LOW);
  }
}
void moving()
{
  Serial.print("Lectura: ");
  Serial.println(analogRead(movimiento));
  if ( analogRead(movimiento) > 900){
  Serial.print("Se detiene el motor");
  act = true;
    return;
  }else{
    Serial.print("Motor andando");
    act = false;
    return;
    }
  }
void controlarBombillo(String url) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
        Serial.printf("[HTTP] GET... código: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            Serial.println(payload);
        }
    } else {
        Serial.printf("[HTTP] GET... falló, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
}