#ifndef TEST_H_
#define TEST_H_

#include <Arduino.h>

/*for continuity testing*/
int contTest(int controlPin, int inputPin, int yLED){
      digitalWrite(controlPin, HIGH); //set transistor
      digitalWrite(yLED, HIGH); //indicate test has started 
      delay(1000);
      int ContSensorValue = digitalRead(inputPin);
      Serial.print("Cont is:\t");
      Serial.println(ContSensorValue);
      digitalWrite(controlPin, LOW); //reset transistor
      digitalWrite(yLED, LOW); //indicate test has ended 
      return ContSensorValue;
}

/*for isolation testing*/
int isoTest(int controlPin, int inputPin, int yLED){
      digitalWrite(controlPin, HIGH); //set transistor
      digitalWrite(yLED, HIGH); //indicate test has started 
      delay(1000);
      int isoSensorValue = digitalRead(inputPin);
      Serial.print("Iso is:\t");
      Serial.println(isoSensorValue);
      digitalWrite(controlPin, LOW); //reset transistor
      digitalWrite(yLED, LOW); //indicate test has ended 
      return isoSensorValue;
}

#endif 
