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
#define pi (3.14159265359)

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

/*
void CoordtoSteps(int coordinate)
{
  //use full steps for large distance, small steps for short distance 
  //function should decelerate when approaching board, then go slow for when probe is pushing against pcb


  //|full step - 1.8 deg per step | 1 | 1/2 step - 0.9 deg per step | 0 | 1/4 step - 0.45 deg per step | 1 | 1/8 step - 0.225 deg per step  | 1 | 1/16 step - 0.1125 deg per step |
  float step_mode_angles[] = {1.8, 0.9, 0.45, 0.225, 0.1125} //in degrees
  float theta = 2*pi*step_mode_angles[0]/360;  //given angle in radians for single step, vary step_mode_angles array val for different step mode

  //s=theta*r  
}
*/

void loop() {                
  delay(100);    // main loop with one second delay
}

void receiveEvent(int howMany) {            // howMANY is always equal to no. of bytes received
                                            //an int is 4 bytes, therefore taking up 4 spaces in the array
  char buf[howMany];
  Serial.print(howMany);
  Serial.println(" bytes");
  for (int i=0; i<=howMany; i++)    // loop through all bytes received
  {
    buf[i] = Wire.read();           // append byte to buffer
  }
  Serial.print("Message received:\n");

  char header = buf[0];
  Serial.print("header:\t");
  Serial.println(header);

  int x;                          // variable to decode to
  memcpy(&x, &buf[1], sizeof(x));       // decode buffer to variable
  Serial.print("x:\t");
  Serial.println(x);                // print the message

  int y;
  memcpy(&y, &buf[5], sizeof(y));
  Serial.print("y:\t");
  Serial.println(y);

  while(stepper.currentPosition() != x)    // Run the motor forward at 200 steps/second until the motor reaches 200 steps (1/16 revolutions):
    {
      stepper.setSpeed(200);          // sets speed in forward direction of stepper 
      stepper.runSpeed();             // runs the step as per speed
    }
 
    delay(1000);                      // delay of 1 second
    
    stepper.setCurrentPosition(0);    // Reset the position to 0:*/

  while(stepper.currentPosition() != y)    // Run the motor forward at 200 steps/second until the motor reaches 200 steps (1/16 revolutions):
    {
      stepper.setSpeed(200);          // sets speed in forward direction of stepper 
      stepper.runSpeed();             // runs the step as per speed
    }
 
    delay(1000);                      // delay of 1 second
    
    stepper.setCurrentPosition(0);    // Reset the position to 0:*/
    
  digitalWrite(LED_BUILTIN, HIGH);  // board LED goes high

  while(stepper.currentPosition() != -100)   // Run the motor back at 200 steps/second until the motor reaches 200 steps (1/16 revolutions):
  {
    stepper.setSpeed(-200);         // sets speed in reverse direction of stepper 
    stepper.runSpeed();             // runs the step as per speed
  }
 
  delay(1000);                      // delay of 1 second
 
  stepper.setCurrentPosition(0);    // Reset new z position to 0:

  Wire.write("done ");              // respond with message of 5 bytes as expected by master 
  Serial.println("Donezo"); 
}


