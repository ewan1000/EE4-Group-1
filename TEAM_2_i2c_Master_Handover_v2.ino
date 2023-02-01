//--------Team 2 SLAVE/MASTER code

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define Z_MOTION 56
#define TEAM1 51
#define TEAM2 52
#define TEAM3 53


byte Z_Offset = 100;
byte MT1 = 0x03;  //Master transfer 1
byte MT2 = 0xAA;
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
  if (MasterInit==1){    //switch stays high  //This is MASTER Statement
    Wire.end();
    Wire.begin(); 
    Serial.begin(9600);
                                                                        //Potential issue: Does the setup function only get called once??
  }   // start serial for output
  else {             //This is SLave Statement
    Wire.end();
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
    Serial.print("Recieved");
  }
  else {
    MasterInit = 0;
      }  
}

void requestEvent(){
        //need to send acknowledgement bit to team 3
  Wire.write(0x01);  //Writes hexadecimal byte (or maybe just int 1) to master //check byte
  //Wire.write(0xAA);     //data byte
}

void loop() {
  switch_val = digitalRead(switch_pin);
  //TEAM2 IS MASTER
  if (MasterInit==1){             //code to execute when master
    while(switch_val==LOW) {
      digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);  //Board LED goes low  
      delay(500);
      Serial.print("Wait ");
    }
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high for Master operation

    //ENTER YOUR TEAMS CODE HERE

    //ENTER YOUR TEAMS CODE HERE

    delay(1000);
    Wire.beginTransmission(Z_MOTION); 
    Wire.write(Z_Offset);        
    Wire.endTransmission(); 
    delay(500);
    
    //Master Handover to Team 3
    delay(10000);
    Wire.beginTransmission(TEAM3);  //This is sending handover to team3, Master2 writing to slave3
    Wire.write(MT1);        
    //Wire.write(MT2);     
    Wire.endTransmission(); 
    MasterInit = 0;
    delay(500);
    setup();
  }
  // TEAM2 IS SLAVE
  else if (MasterInit==0) {
    delay(100); 
    if (switch_val==HIGH){
      Serial.print("Wait slave");
      delay(100);
      Wire.end();
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);  //Board LED goes low  
      delay(250);
      setup(); 
    }
    else {
      Serial.print("Wait s2");
      delay(100);
      setup();
    }
  }
}


/////Look into setup(), is this possible to be called 
//check if the team1 is writing to team2 properly to send masterpacket 
