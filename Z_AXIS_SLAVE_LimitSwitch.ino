// Slave Reader running on Device No.2 (Use with corresponding Master Writer running on Device No.1)

// function that executes whenever data is received from master

#include "hardware/i2c.h"
#include <AccelStepper.h>
#include <Wire.h> 

#define motorInterfaceType 1

// define pin number allocation
#define Z_MOTION 56       // z-motion i2c address
#define dirPin 16         // motor direction
#define stepPin 17        // step enable
#define stepMs1Pin 21     // M1 A4988 step enable
#define stepMs2Pin 20     // M2 A4988 step enable
#define stepMs3Pin 19     // M3 A4988 step enable
#define upperLimit_pin 0
#define lowerLimit_pin 1


const int stepsPerRevolution = 3200;
int Z_Origin = 0; //not at origin when z equals 0
int Z_Failsafe = 0;

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup()
 {
    attachInterrupt(digitalPinToInterrupt(upperLimit_pin),readUpper_LimitSwitch,RISING); //Interrupt called on upper_limit pin on logic rising from low to high
    attachInterrupt(digitalPinToInterrupt(lowerLimit_pin),readLower_LimitSwitch,RISING); //Interrupt called on switch pin on logic rising from low to high
    stepper.setMaxSpeed(2000);              // stepper max speed of steps per second
    pinMode(stepMs1Pin, OUTPUT);            // set Ms1Pin as an Outpu, Pin (21)
    pinMode(stepMs2Pin, OUTPUT);            // set Ms2Pin as an Outpu, Pin (20)         
    pinMode(stepMs3Pin, OUTPUT);            // set Ms3Pin as an Outpu, Pin (19)
    pinMode(LED_BUILTIN, OUTPUT);           // Built in LED goes high to indicate opperation function
    digitalWrite(stepMs1Pin,HIGH);          // microstepping A4988 pins MS1: 0 |full step, | 1 | 1/2 step, | 0 | 1/4 step, | 1 | 1/8 step, | 1 | 1/16 step,
    digitalWrite(stepMs2Pin,HIGH);          // microstepping A4988 pins MS2: 0 |full step, | 0 | 1/2 step, | 1 | 1/4 step, | 1 | 1/8 step, | 1 | 1/16 step,
    digitalWrite(stepMs3Pin,HIGH);          // microstepping A4988 pins MS3: 0 |full step, | 0 | 1/2 step, | 0 | 1/4 step, | 0 | 1/8 step, | 1 | 1/16 step,
    gpio_set_function(4, GPIO_FUNC_I2C);    // initializes GPIO pin 4 as the I2C as the SDA line
    gpio_set_function(5, GPIO_FUNC_I2C);    // initializes GPIO pin 4 as the I2C as the SCL line
    gpio_pull_up(4);                        // initializes internal pull-up resistor to GPIO pin 4
    gpio_pull_up(5);                        // initializes internal pull-up resistor to GPIO pin 5
    Wire.begin(Z_MOTION);                   // join i2c bus with address #56
    Wire.onReceive(receiveEvent);           // register event
    Wire.onRequest(requestEvent);
    Serial.begin(9600);                     // start serial for output
    Serial.printf("Slave Sketch here...");  // Serial Skave function check  
  }

void loop() 
  {                
   delay(100);    // main loop with one second delay  
  }



void readUpper_LimitSwitch()  //used for indicating that the rack has reached the homepoint
  {
    stepper.setCurrentPosition(0);   
    Z_Origin = 1;
    //interrupt triggered when upper limit reached, resets position to 0  
    //now the 
  } 

void readLower_LimitSwitch()  
  {
    stepper.setMaxSpeed(0);              // stepper max speed of steps per second
    Z_Failsafe = 1;
  } 


void requestEvent()
  {
    if (stepper.currentPosition() == 0)
      {
        Wire.write(1); //sending number 1 once stepper.currentPosition 
      }
    else
      {
        Wire.write(0);
      } 
  }

void receiveEvent(int howMany) // howMANY is always equal to no. of bytes received
  {                             //an int is 4 bytes, therefore taking up 4 spaces in the array                                    
    Serial.println("Transmission from master device detected!");
    Serial.print("Reading message...\t");
    char buf[howMany];              //creating a character storage buffer
    Serial.print(howMany);
    Serial.println(" bytes");       //no. of received bytes along i2c bus
    for (int i=0; i<=howMany; i++)    // loop through all bytes received
      {
        buf[i] = Wire.read();           // append byte to buffer
      }
    Serial.print("Message received:\n");

    char header = buf[0];           //only element 0 as a char is 1 byte
    Serial.print("header:\t");
    Serial.println(header);

    int z1;                          
    memcpy(&z1, &buf[1], sizeof(z1));    //decoding the character to an int, from elements 1-4 of the buffer, since int=4 bytes
    Serial.print("z1:\t");
    Serial.println(z1);                //print message to check
    if (z1 >= 0)
      {
        while((stepper.currentPosition() != z1) || (Z_Failsafe==0))    // Run the motor forward at 1500 steps/second until the motor reaches coord from array (1/16 revolutions)
          {
            stepper.setSpeed(1500);         // sets speed in forward direction of stepper 
            stepper.runSpeed();             // runs the step as per speed
          }
      }
    else
      {
       while((stepper.currentPosition() != z1) || (Z_Origin==0))    // Run the motor backward at 1500 steps/second until the motor reaches coord from array (1/16 revolutions)
          {
            stepper.setSpeed(-1500);         // sets speed in forward direction of stepper 
            stepper.runSpeed();             // runs the step as per speed
          }    
      }  
    if (Z_Failsafe==1)
      {
        while(Z_Origin==0)    // Run the motor backward at 1500 steps/second until the motor reaches coord from array (1/16 revolutions)
          {
            stepper.setSpeed(-1500);         // sets speed in forward direction of stepper 
            stepper.runSpeed();             // runs the step as per speed
          }    
      }
    Z_Failsafe=0;
    Z_Origin=0; 
    delay(1000); 
    stepper.setCurrentPosition(0);
    digitalWrite(LED_BUILTIN, HIGH);  // board LED goes high
    Serial.println("Donezo"); 

  }


