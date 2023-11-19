void getTemp()
{
  tempC = thermocouple.readCelsius();
  ThingSpeak.setField(1, tempC);
}
void alarm()
{
  static unsigned long startTime = 0;
  unsigned long currentTime = millis();
   if (currentTime - startTime < 2500){
    if(digitalRead(5)==LOW){
      digitalWrite(5, HIGH);
    }
   } else{
    digitalWrite(5, LOW);
    startTime = currentTime;
   } 
}
void fire(){
  //Cuando enciende
  if(tempC<temperaturaMaxima && tempC<temperaturaMinima){
    Serial.println("Encendiendo flautas");
    digitalWrite(4,HIGH);
  }
  //Cuando apaga
  else{
    Serial.println("Apagando flautas");
    digitalWrite(4,LOW);
    alarm();
  }
}


