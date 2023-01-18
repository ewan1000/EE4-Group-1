#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Stepper.h>
#include <dummy_rp2040.h>

// define pin number allocation
#define dirPin 19
#define stepPin 20
#define stepMs1Pin 14
#define stepMs2Pin 15
#define stepMs3Pin 16

void setup() {
  // declare pins as outputs
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(stepMs1Pin, OUTPUT);
  pinMode(stepMs2Pin, OUTPUT);
  pinMode(stepMs3Pin, OUTPUT);

}

void loop(){
  // step forward 360 degree rotation
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < 200; x++)  {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);  
  }
  delay(1000); //one second delay

  // step backwards -720 degree rotations
  digitalWrite(dirPin, LOW);
  for(int x = 0; x < 400; x++);  {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);  
  }
  delay(1000);

  // half step forward 180 degree rotation
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < 200; x++)  {
    digitalWrite(stepPin,HIGH);
    digitalWrite(stepMs1Pin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    digitalWrite(stepMs1Pin,LOW);
    delayMicroseconds(500);  
  }
  delay(1000); //one second delay

  // quarter step forward 90 degree rotation
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < 200; x++)  {
    digitalWrite(stepPin,HIGH);
    digitalWrite(stepMs2Pin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    digitalWrite(stepMs2Pin,LOW);
    delayMicroseconds(500);  
  }
    delay(1000); //one second delay

  // eigth step forward 45 degree rotation
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < 200; x++)  {
    digitalWrite(stepPin,HIGH);
    digitalWrite(stepMs1Pin,HIGH);
    digitalWrite(stepMs2Pin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    digitalWrite(stepMs1Pin,LOW);
    digitalWrite(stepMs2Pin,LOW);
    delayMicroseconds(500);  
  }
    delay(1000); //one second delay

  // eigth step forward 22.5 degree rotation
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < 200; x++)  {
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
}
