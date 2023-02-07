// Slave Reader running on Device No.2 (Use with corresponding Master Writer running on Device No.1)

// function that executes whenever data is received from master

#include "hardware/i2c.h"
#include <AccelStepper.h>
#include <Wire.h> 

#define motorInterfaceType 1

// define pin number allocation
#define Z_MOTION 56       // z-motion i2c address
#define dirPin 14         // motor direction
#define stepPin 15        // step enable
#define stepMs1Pin 10     // M1 A4988 step enable
#define stepMs2Pin 11     // M2 A4988 step enable
#define stepMs3Pin 12     // M3 A4988 step enable

const int stepsPerRevolution = 3200;


AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  stepper.setMaxSpeed(1000);              // stepper max speed of steps per second
  pinMode(stepMs1Pin, OUTPUT);            // set Ms1Pin as an Outpu, Pin (10)
  pinMode(stepMs2Pin, OUTPUT);            // set Ms2Pin as an Outpu, Pin (11)         
  pinMode(stepMs3Pin, OUTPUT);            // set Ms3Pin as an Outpu, Pin (12)
  pinMode(LED_BUILTIN, OUTPUT);           // Built in LED goes high to indicate opperation function
  digitalWrite(stepMs1Pin,HIGH);          // microstepping A4988 pins MS1: 0 |full step, | 1 | 1/2 step, | 0 | 1/4 step, | 1 | 1/8 step, | 1 | 1/16 step,
  digitalWrite(stepMs2Pin,HIGH);          // microstepping A4988 pins MS2: 0 |full step, | 0 | 1/2 step, | 1 | 1/4 step, | 1 | 1/8 step, | 1 | 1/16 step,
  digitalWrite(stepMs3Pin,HIGH);          // microstepping A4988 pins MS3: 0 |full step, | 0 | 1/2 step, | 0 | 1/4 step, | 0 | 1/8 step, | 1 | 1/16 step,
  gpio_set_function(4, GPIO_FUNC_I2C);    // initializes GPIO pin 4 as the I2C as the SDA line
  gpio_set_function(5, GPIO_FUNC_I2C);    // initializes GPIO pin 4 as the I2C as the SCL line
  gpio_pull_up(4);                        // initializes internal pull-up resistor to GPIO pin 4
  gpio_pull_up(5);                        // initializes internal pull-up resistor to GPIO pin 5
  Wire.begin(Z_MOTION);                   // join i2c bus with address #4
  Wire.onReceive(receiveEvent);           // register event
  Serial.begin(9600);                     // start serial for output
  Serial.printf("Slave Sketch here...");  // Serial Skave function check
}

void loop() {                
  delay(1000);    // main loop with one second delay
}

void receiveEvent(int) {            // when recieving an event from Master
  int Data = Wire.read();           // receive byte as an integer
  Serial.println(Data);             // print the integer
  digitalWrite(LED_BUILTIN, HIGH);  // board LED goes high

  stepper.setCurrentPosition(0);    // Set the current z position to 0:
 
  while(stepper.currentPosition() != Data)    // Run the motor forward at 200 steps/second until the motor reaches 200 steps (1/16 revolutions):
  {
    stepper.setSpeed(200);          // sets speed in forward direction of stepper 
    stepper.runSpeed();             // runs the step as per speed
  }
 
  delay(1000);                      // delay of 1 second
 
  stepper.setCurrentPosition(0);    // Reset the position to 0:
 


  while(stepper.currentPosition() != -Data)   // Run the motor back at 200 steps/second until the motor reaches 200 steps (1/16 revolutions):
  {
    stepper.setSpeed(-200);         // sets speed in reverse direction of stepper 
    stepper.runSpeed();             // runs the step as per speed
  }
 
  delay(1000);                      // delay of 1 second
 
  stepper.setCurrentPosition(0);    // Reset new z position to 0:

  Wire.write("done ");              // respond with message of 5 bytes as expected by master 
}

