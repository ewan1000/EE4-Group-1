
//------------------------------------------------slave code

#include "hardware/i2c.h"
#include <AccelStepper.h>
#include <Wire.h>

#define motorInterfaceType 1
// define pin number allocation
#define dirPin 14
#define stepPin 15
#define stepMs1Pin 10
#define stepMs2Pin 11
#define stepMs3Pin 12

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

int Data;

void setup(){
pinMode(LED_BUILTIN, OUTPUT);
gpio_set_function(4, GPIO_FUNC_I2C);
gpio_set_function(5, GPIO_FUNC_I2C);
gpio_pull_up(4);
gpio_pull_up(5);
Wire.begin(8);                // join i2c bus with address #8
Wire.onRequest(requestEvent); // register event
Serial.begin(9600);
Serial.println( "The Master sketch has started");

}

void receiveEvent(){
 Data = Wire.read();
}
void requestEvent() {
    digitalWrite(LED_BUILTIN, HIGH);
       int a = Data;
       //long i = a;
  // eigth step forward 22.5 degree rotation
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < a; a++)  {
        digitalWrite(stepPin,HIGH);
        digitalWrite(stepMs1Pin,HIGH);
        digitalWrite(stepMs2Pin,HIGH);
        digitalWrite(stepMs3Pin,HIGH);
        delayMicroseconds(500);
        digitalWrite(stepPin,LOW);
        digitalWrite(stepMs1Pin,LOW);
        digitalWrite(stepMs2Pin,LOW);
        digitalWrite(stepMs3Pin,LOW);
        delayMicroseconds(500);  
  }
    delay(1000); //one second delay
  Wire.write("done "); // respond with message of 6 bytes
  // as expected by master
}
void loop() {
delay(500);
}
