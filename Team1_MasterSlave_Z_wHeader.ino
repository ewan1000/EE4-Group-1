//--------Team 1 SLAVE/MASTER code

#include "Z_Control_Functions.h"
#include "hardware/i2c.h"
#include <Wire.h>
#include <AccelStepper.h>
#include <math.h>


#define motorInterfaceType 1
// define pin number allocation
#define dirPin 14
#define stepPin 15
#define stepMs1Pin 10
#define stepMs2Pin 11
#define stepMs3Pin 12

#define I2C_PORT i2c0
#define TEAM1 51
#define TEAM2 52
#define TEAM3 53
#define TEAM4 54
#define XY_MOTION 55
#define Z_MOTION 56

byte Z_Offset = 100;
byte MT1 = 3;  //Master transfer 1 byte, stage handover byte
//byte MT2 = 0xAA;

int R_LED_MASTER = 17;
int G_LED_SLAVE = 16;
int MASTERswitchSTATE;  //the switch only turns the MCU into a master, slave is activated automatically
int MasterInit = 1;      //Team1 is initially master 
int switch_pin = 1;  //switch is GPIO 1

void setup()    
  {
    attachInterrupt(digitalPinToInterrupt(switch_pin),readSwitch,RISING); //Interrupt called on switch pin on logic rising from low to high
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(switch_pin,INPUT);
    pinMode(R_LED_MASTER, OUTPUT);
    pinMode(G_LED_SLAVE, OUTPUT);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);      //sets up pull-up resistors on I2C pins, essential for I2C
    gpio_pull_up(5);
    if (MasterInit == 1)
      {     //Master Setup: Team1 is configured to Master on I2C0 channel
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
    int sVal = digitalRead(switch_pin);
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
            digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high for Master operation
            digitalWrite(G_LED_SLAVE, LOW);  //Slave LED goes LOW
            digitalWrite(R_LED_MASTER, LOW);  //Master state LED goes LOW 
            Serial.println("Flick Switch to change state and send byte");
            delay(2000);
            setup(); 
          }
        else if (MASTERswitchSTATE == 1)
          {
            Serial.println("Team1 Master is ready to transmit");
            digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high for Master operation
            digitalWrite(G_LED_SLAVE, LOW);  //Slave LED goes LOW
            digitalWrite(R_LED_MASTER, HIGH);  //Master state LED goes HIGH

            int coord_arr[] = {62000,10000}; 
            int coord_num = sizeof(coord_arr)/4;  //since coord always integer, can divide total number of bytes to find number of elements
            Serial.print("Writing message:\n");

            for (int i=0;i<coord_num;i++)
              {   
                Z_Coord_Sender(coord_arr[i]);
                Serial.println("Sent z-coordinate");

            //ENTER YOUR TEAMS CODE HERE
            //MAX Z-Offset is 63000 um
                Serial.println("Now let's wait to simulate doing stuff");
                delay(1000);
                Serial.println("Humdy humdy hum");
                Serial.println("Dimdy Dumdy dee");
                Serial.println("We did a thing!!");
                delay(100);                                      
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
            digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW for Slave operation
            digitalWrite(G_LED_SLAVE, HIGH);  //Slave LED goes HIGH
            digitalWrite(R_LED_MASTER, LOW);  //Master state LED goes LOW
            delay(2000);
            setup(); 
          }
        else if (MASTERswitchSTATE == 0) 
          {
            Serial.println("Team1 is now a slave ");
            digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW for Slave operation
            digitalWrite(G_LED_SLAVE, HIGH);  //Slave LED goes HIGH
            digitalWrite(R_LED_MASTER, LOW);  //Master state LED goes LOW
            delay(2000);
            setup();
          }
      }
  } 
