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
  if (tempC < temperaturaMinima)
  {
    Serial.println("Encendiendo flautas");
    digitalWrite(4, HIGH);
    return;
  }
  // Cuando se apaga
  else if(tempC> temperaturaMaxima || digitalRead(13) == LOW || !act){
    Serial.println("Apagando flautas");
    digitalWrite(4, LOW);
    alarm();
  }
}
void move(){
  digitalWrite(18, HIGH);
  delay(1);
  digitalWrite(18, LOW);
  DURACION = pulseIn(19, HIGH);
  DISTANCIA = DURACION / 18.7;
  delay(500);
  if (DISTANCIA <= 200 && DISTANCIA >= 20) {
    Serial.println("Objeto detectado");
    act= true;
    return;
  }
  else{
    Serial.println("Objeto no detectado");
    act= false;
    return;
  }
}
