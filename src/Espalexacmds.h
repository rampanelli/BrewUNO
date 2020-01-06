void AlexaCommands(EspalexaDevice* espalexaPointer) { 
  if(espalexaPointer == nullptr) return;
  int AlexaPercent = espalexaPointer->getPercent();

  if (espalexaPointer->getName() == "Aquecimento") {
        analogWrite(HEATER_BUS, (AlexaPercent*1023)/100);
        Serial.print("Set Heater PWM : ");
        Serial.println((AlexaPercent*1023)/100);
  }

    if (espalexaPointer->getName() == "Sparge") {
        analogWrite(SPARGE_HEATER_BUS, (AlexaPercent*1023)/100);
        Serial.print("Set Sparge PWM : ");
        Serial.println((AlexaPercent*1023)/100);
  }

  if (espalexaPointer->getName() == "Bomba") {
        analogWrite(PUMP_BUS, (AlexaPercent*1023)/100);
        Serial.print("Set Pump PWM : ");
        Serial.println((AlexaPercent*1023)/100);
  }
  
}