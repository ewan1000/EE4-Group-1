//--------Team 1 SLAVE/MASTER code

#include "Z_Sender.h"
#include "test.h"
#include "led.h"

#include "hardware/i2c.h"
#include <Arduino.h>
#include <Wire.h>
#include <vector>


#define I2C_PORT i2c0
#define TEAM1 51
#define TEAM2 52
#define TEAM3 53
#define TEAM4 54
#define XY_MOTION 55
#define Z_MOTION 56

byte MT1 = 3;  //Master transfer 1 byte, stage handover byte

//BELOW ARE ALLOCATED PINS
int MASTERswitchSTATE;  //the switch only turns the MCU into a master, slave is activated automatically
int MasterInit = 1;      //Team1 is initially master 

int contTransistor = 27;
int contInput = 26;
int isoTransistor = 17;
int isoInput = 16;
int zTrigger = HIGH; //simulated feedback from Z
int xyTrigger = HIGH; //simulated feedback from XY

int builtInLED = 25;

int I2CGreenLED = 6;
int I2CRedLED = 7;
int switchIn = 8;

int isoRedLED = 13;
int isoYellowLED = 14;
int isoGreenLED = 15;

int contRedLED = 10;
int contYellowLED = 11;
int contGreenLED = 12;


bool pointFail = false;
bool testToggle = false; 
//bool isActive = false;
int isoTestCounter = 0;
int contTestCounter = 0; 


int coor [3][3] = {{1,2,3}, {4,5,6}, {7,8,9}}; //coordinate simulation
int xy[] = {200, 300, 400, 500, 600, 700, 800, 900}; //seperate XY and Z coordinates


std :: vector <int> failedPoints; //vector to store location of failed test points

void setup()    
  {
    attachInterrupt(digitalPinToInterrupt(switchIn),readSwitch,RISING); //Interrupt called on switch pin on logic rising from low to high
    pinMode(LED_BUILTIN, OUTPUT);

  //intialise all the pinModes and transistor control set
    //pinMode(trigger, INPUT); 
    pinMode(contTransistor, OUTPUT);
    pinMode(contInput, INPUT);
    pinMode(builtInLED, OUTPUT);
    pinMode(isoTransistor, OUTPUT);
    pinMode(isoInput, INPUT);

    pinMode(I2CGreenLED, OUTPUT);
    pinMode(I2CRedLED, OUTPUT);
    pinMode(switchIn, INPUT);

    pinMode(isoRedLED, OUTPUT);
    pinMode(isoYellowLED, OUTPUT);
    pinMode(isoGreenLED, OUTPUT);

    pinMode(contRedLED, OUTPUT);
    pinMode(contYellowLED, OUTPUT);
    pinMode(contGreenLED, OUTPUT);

    digitalWrite(contTransistor, LOW);
    digitalWrite(isoTransistor, LOW);

    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);      //sets up pull-up resistors on I2C pins, essential for I2C
    gpio_pull_up(5);
    if (MasterInit == 1)
      {     //Master Setup: Team1 is configured to Master on I2C0 channel
        testLEDs(I2CGreenLED, I2CRedLED, isoGreenLED, isoYellowLED, isoRedLED, contGreenLED, contYellowLED, contRedLED);
        Wire.end();  
        Wire.begin(); 
        Serial.begin(9600);
      }   
    else if(MasterInit == 0)
      {            //Slave Setup: Team1 is configured to Slave on I2C0 channel
        Wire.end();
        Wire.begin(TEAM1);
        Serial.begin(9600); 
        Wire.onReceive(receiveEvent);
      }   
     
  }

void receiveEvent(int howMany)
  {   // howMANY is always equal to no. of bytes received
    int Slave1Receives = Wire.read();       
    //reads handover byte from master4
    if (Slave1Receives == 3 )
      {          //if team1 as slave receives a 3 from other team acting as master
      Serial.println(3);
      MasterInit = 1;                 //Now team1 can be initialised as master instead of slave
      Serial.println("Received Byte from Master");
      setup(); 
      }
    else 
      {
      MasterInit = 0;
      }  
  }


void readSwitch()  
  {
    int sVal = digitalRead(switchIn);
    if (sVal == 1) 
      {
        MASTERswitchSTATE = 1;        
      }
    else
      {
        MASTERswitchSTATE = 0;
      }
  } 



void loop() 
  {
    //TEAM1 has been initialised as MASTER
    if (MasterInit==1)
      {        
        Serial.println("Master! ");            //This condition is for swapping team's head
        if (MASTERswitchSTATE == 0) 
          {
            digitalWrite(I2CGreenLED, LOW);  //Slave LED goes LOW
            digitalWrite(I2CRedLED, LOW);  //Master state LED goes LOW 
            Serial.println("Flick Switch to change state and send byte");
            delay(2000);
            setup(); 
          }
        else if (MASTERswitchSTATE == 1)
          {
            Serial.println("Team1 Master is ready to transmit");
            digitalWrite(I2CGreenLED, LOW);  //Slave LED goes LOW
            digitalWrite(I2CRedLED, HIGH);  //Master state LED goes HIGH

            int zOffsetHIGH = 30000;
            int zOffsetLOW = 36000;

            int coord_arr[] = {zOffsetLOW,zOffsetLOW,zOffsetLOW,zOffsetLOW,zOffsetLOW,zOffsetLOW,zOffsetLOW,zOffsetLOW}; 
            int coord_num = sizeof(coord_arr)/4;  //since coord always integer, can divide total number of bytes to find number of elements
            Serial.print("Writing message:\n");

            for (int i=0;i<coord_num;i++)
              {   
                Z_Coord_Sender(coord_arr[i]);

              //isActive = digitalRead(trigger);
                Serial.print("Sending XY coordinate\t"); Serial.print(i); Serial.println(i+1);
                delay(1000);
                if (xyTrigger == HIGH){
                  Serial.print("Sending Z coordinate\t"); Serial.println(zOffsetLOW);
                  delay(1000);
                  resetLEDs(isoGreenLED, isoYellowLED, isoRedLED, contGreenLED, contYellowLED, contRedLED); // reset LEDs

                  if (testToggle == false && zTrigger == HIGH){ //test state is continutity
                    pointFail = false; //reset 
                    if (contTest(contTransistor, contInput, contYellowLED) == 1){ //
                      digitalWrite(contGreenLED, HIGH); //indicate test has passed 
                      Serial.println("Cont Test Passed");
                      testToggle = true;
                      contTestCounter = 0;
                    }
                    else {
                      digitalWrite(contRedLED, HIGH); //indicate test has failed
                      Serial.println("Cont Soft Fail Re-testing");
                      contTestCounter += 1;

                    }
                    if (contTestCounter >= 5){
                      Serial.println("Cont Test Hard Failed, storing failed coordinates");
                      flashLED(contRedLED);
                      testToggle = true;
                      contTestCounter = 0;
                      //exit(0);
                      failedPoints.push_back(xy[i]);
                      failedPoints.push_back(xy[i+1]);
                      pointFail = true;
                    }


                
                  }



                  else{ //test state iso
                    resetLEDs(isoGreenLED, isoYellowLED, isoRedLED, contGreenLED, contYellowLED, contRedLED); // reset LEDs
                    if (isoTest(isoTransistor, isoInput, isoYellowLED) == 1){
                      Serial.println("Iso Test Passed"); Serial.print("Sending Z coordinate\t"); Serial.println(zOffsetHIGH);
                      digitalWrite(isoGreenLED, HIGH); //indicate test has passed
                      testToggle = false;
                      isoTestCounter = 0;
                    }
                    else {
                      digitalWrite(isoRedLED, HIGH); //indicate test has failed
                      Serial.println("Iso Soft Fail Re-testing");
                      isoTestCounter += 1;
                    }
                    if (isoTestCounter >= 5){
                      Serial.println("Iso Test Hard Failed");
                      flashLED(isoRedLED);
                      testToggle = false;
                      isoTestCounter = 0;
                      if (pointFail == false){
                        failedPoints.push_back(xy[i]);
                        failedPoints.push_back(xy[i+1]);
                        Serial.println("Storing failed coordinates");
                      
                      
                      }
                      //exit(0);
                    }

                  }

                }
                Z_Coord_Sender(zOffsetHIGH);
              }
              
            Z_Coord_Sender(0); //this is to reset to home point after team is finished
            Serial.println("Finished, now going to origin");
            Serial.println("Terminating transmission...");
            Serial.println("Transmission terminated.");
                
            //Master Handover to Team 2
            delay(2000);
            Wire.beginTransmission(TEAM2);  //This is sending handover to team2, Master1 writing to slave2
            Wire.write(MT1);        //WRITING MT1 BYTE = 3
            Wire.endTransmission(); 
            delay(100);
            Serial.println("GOOD: Team1 has sent MT1 byte to team 2 ");
            delay(1000);
            MasterInit = 0;
            MASTERswitchSTATE = 0; //this is for 2nd time around, so that switch does automatically turn on
            setup();
          }
      }
    // TEAM 1 has been initialised as SLAVE
    else if (MasterInit == 0) 
      {
        Serial.println("SLAVE! ");  
        if (MASTERswitchSTATE == 1)
          {
            Serial.println("Team1 is now a slave ");
            digitalWrite(I2CGreenLED, HIGH);  //Slave LED goes HIGH
            digitalWrite(I2CRedLED, LOW);  //Master state LED goes LOW
            delay(2000);
            setup(); 
          }
        else if (MASTERswitchSTATE == 0) 
          {
            Serial.println("Team1 is now a slave ");
            digitalWrite(I2CGreenLED, HIGH);  //Slave LED goes HIGH
            digitalWrite(I2CRedLED, LOW);  //Master state LED goes LOW
            delay(2000);
            setup();
          }
      }
  } 
