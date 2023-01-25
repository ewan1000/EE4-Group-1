// Slave Reader running on Device No.2 (Use with corresponding Master Writer running on Device No.1)

// function that executes whenever data is received from master

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

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
  Wire.begin(8);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  Serial.printf("Slave Sketch here...");
}

void loop() {
  delay(500);
}

void receiveEvent(int howMany) {   //not sure what int howMany is for
  int Data = Wire.read();          // receive byte as an integer
  Serial.println(Data);            // print the integer
  digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < Data; x++)  {
    digitalWrite(stepPin,HIGH);        //Forward
    digitalWrite(stepMs1Pin,HIGH);     //microstepping pins: 16th step
    digitalWrite(stepMs2Pin,HIGH);
    digitalWrite(stepMs3Pin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);         //Reverse
    digitalWrite(stepMs1Pin,LOW);      //microstepping pins: Full step (200 step/revol) - 1.8 degrees
    digitalWrite(stepMs2Pin,LOW);
    digitalWrite(stepMs3Pin,LOW);
    delayMicroseconds(500);  
  }
  delay(1000); //one second delay
  Wire.write("done "); // respond with message of 5 bytes
  // as expected by master   
}