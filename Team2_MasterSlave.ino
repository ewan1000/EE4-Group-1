//--------Team 2 SLAVE/MASTER code

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define Z_MOTION 56
#define TEAM1 51
#define TEAM2 52
#define TEAM3 53


byte Z_Offset = 255;
byte MT1 = 0x03;  //Master transfer 1
byte MT2 = 0xAA;
int SLAVE_INIT;
int digitalSLAVE=0;
int MasterInit=0;
int switch_pin = 6;
int switch_val;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(switch_pin,INPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5); 
  switch_val = digitalRead(switch_pin);
  if ((switch_val==HIGH) && (MasterInit==1)){    //switch stays high  //This is MASTER Statement
    Wire.begin(); 
    Serial.begin(9600);
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
                                                                        //Potential issue: Does the setup function only get called once??
  }   // start serial for output
  else if ((switch_val==LOW) && (MasterInit==0)) {             //This is SLave Statement
    Wire.begin(TEAM2);
    Serial.begin(9600); 
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    }   // start serial for output

}

void receiveEvent(int howmany){
   while(Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  } 
  int MasterSend1 = Wire.read();       //reads handover byte from master1
 // int MasterSend2 = Wire.read();   //remember to use this to match up to protocol
  if (MasterSend1 == MT1 ){
    MasterInit = 1;
  }
  else {
    MasterInit = 0;
      }  
}

void requestEvent(int howmany){
        //need to send acknowledgement bit to team 3
  Wire.write(0x01);  //Writes hexadecimal byte (or maybe just int 1) to master //check byte
  Wire.write(0xAA);     //data byte
}

void loop() {
  switch_val = digitalRead(switch_pin);
  //TEAM2 IS MASTER
  if ((switch_val==HIGH) && (MasterInit==1)){              //code to execute when master
    delay(1000);
    Wire.beginTransmission(Z_MOTION); 
    Wire.write(x);        
    Wire.endTransmission(); 
    delay(500);
    
    
    delay(1000);
    Wire.beginTransmission(TEAM3);  //This is sending handover to team3, Master2 writing to slave3
    Wire.write(MT1);        
    Wire.write(MT2);     
    Wire.endTransmission(); 
    digitalSLAVE = 1;
    delay(500);
    setup();
  }
  //TEAM2 IS SLAVE
  else if ((switch_val==LOW) && (MasterInit==0)){
    delay(100);   
    setup();            //slave stuff
  }
}
