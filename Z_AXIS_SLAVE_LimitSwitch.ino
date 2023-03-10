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
    stepper.setSpeed(0);  //ideally stops motor from moving 
    stepper.runSpeed();   //these may need to be inside while statements
    stepper.setCurrentPosition(0);   
    Z_Origin = 1;
    Serial.println("At the origin");
    requestEvent(); //calling this to send a 1 once    
    //interrupt triggered when upper limit reached, resets position to 0  
  } 

void readLower_LimitSwitch()  
  { 
    Z_Failsafe = 1;
    Serial.println("At end of Z-Axis");
  } 

int Displacement_Step_Converter_Z(int displacement_um) //function returns the step value after converting um input displacement
  {
    float pinion_circumference = 97390; //this is in um //this is been calculated by hand (C = pi*D)
    int step_resolution = round(pinion_circumference/3200);
    int StepValue = round(displacement_um/step_resolution);
    return StepValue;
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
    z1 = Displacement_Step_Converter_Z(z1);
    Serial.print("Motor Steps:"); 
    Serial.println(z1);

    //This set of if/else if statement is for typical movement
    if (z1 >= 0)
      {
        while((stepper.currentPosition() != z1) && (Z_Failsafe==0) && (Z_Origin==0))    //move down from origin when not at position and not triggered failsafe
          {
            stepper.setSpeed(150);         // sets speed in forward direction of stepper 
            stepper.runSpeed();             // runs the step as per speed
          }
      }
    else if (z1 < 0)
      {
       while((stepper.currentPosition() != z1) && (Z_Origin==0) && (Z_Failsafe==0))    //move up from origin when not at position and not triggered failsafe
          {
            stepper.setSpeed(-150);         // sets speed in forward direction of stepper 
            stepper.runSpeed();             // runs the step as per speed
          }
      }  

    //This if/else occurs when lower limit switch (failsafe) has been triggered
    if (Z_Failsafe==1 && Z_Origin==0)
      {
        while(Z_Origin==0)
          { 
            stepper.setSpeed(-50);         // motor has hit lower limitswitch, moves back to origin slowly
            stepper.runSpeed();                   
          }           
      }
    else 
      {                        
      }                               
    
    //can't include these in same if else structure as it will not enter the else if when Z_Origin changes value
    
    //This if/else occurs when upper limit switch has been triggered, after the lower limit switch has been hit
    if (Z_Failsafe==1 && Z_Origin==1)
      {
        stepper.setSpeed(0);         // motor has hit upper limitswitch now, stop at origin 
        stepper.runSpeed(); 
        stepper.setCurrentPosition(0);  //Could potentially remove this loop 
      }  
    else 
      {
      }  
    Z_Failsafe=0;
    Z_Origin=0; 
    delay(1000); 
    stepper.setCurrentPosition(0);
    Serial.println("Donezo"); 

  }


