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
const int stepsPerRevolution = 3200;


AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  stepper.setMaxSpeed(1000);
  digitalWrite(stepMs1Pin,HIGH);     //microstepping pins: 16th step
  digitalWrite(stepMs2Pin,HIGH);
  digitalWrite(stepMs3Pin,HIGH);
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
   // Set the current position to 0:
  stepper.setCurrentPosition(0);
 
  // Run the motor forward at 200 steps/second until the motor reaches 400 steps (2 revolutions):
  while(stepper.currentPosition() != 400)
  {
    stepper.setSpeed(200);
    stepper.runSpeed();
  }
 
  delay(1000);
 
  // Reset the position to 0:
  stepper.setCurrentPosition(0);

}

void receiveEvent(int howMany) {   //not sure what int howMany is for
  int Data = Wire.read();          // receive byte as an integer
  Serial.println(Data);            // print the integer
  digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
  // Set the current position to 0:
  stepper.setCurrentPosition(0);
 
  // Run the motor forward at 200 steps/second until the motor reaches 400 steps (2 revolutions):
  while(stepper.currentPosition() != 400)
  {
    stepper.setSpeed(200);
    stepper.runSpeed();
  }
 
  delay(1000);
 
  // Reset the position to 0:
  stepper.setCurrentPosition(0);
  Wire.write("done "); // respond with message of 5 bytes
  // as expected by master   
}
