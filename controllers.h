
String getRequest(const char* serverName)
{
  HTTPClient http;
  http.begin(serverName);

  int httpResponseCode = http.GET();

  String payload = "...";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else{
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}
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
    delay(requestInterval);
    answer = getRequest(RelayBombilloOff);
    Serial.println(answer);
    flauta = "off";
    alarm();
  }
  if (digitalRead(13) == LOW || !act)
  {
    Serial.println("Apagando flautas");
    delay(requestInterval);
    answer = getRequest(RelayBombilloOff);
    Serial.println(answer);
    flauta = "off";
    alarm();
  }
  // Cuando enciende
  if (tempC < temperaturaMinima)
  {
    Serial.println("Encendiendo flautas");
    delay(requestInterval);
    answer = getRequest(RelayBombilloOn);
    Serial.println(answer);
    flauta = "on";
    digitalWrite(5, LOW);
  }
}
void moving()
{
  Serial.print("Lectura: ");
  Serial.println(analogRead(movimiento));
  if ( analogRead(movimiento) > 900){
  Serial.print("Se detuvo el motor");
  act = true;
    return;
  }else{
    Serial.print("Motor andando");
    act = false;
    return;
    }
  }

