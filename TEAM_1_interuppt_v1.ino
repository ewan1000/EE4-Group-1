//--------Team 1 SLAVE/MASTER code

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define Z_MOTION 56
#define TEAM1 51
#define TEAM2 52
#define TEAM3 53
#define Mswitch 1 

byte Z_Offset = 100;
byte MT1 = 3;  //Master transfer 1
//byte MT2 = 0xAA;
int MasterInit = 1;
int switch_pin = 0;
int MASTERswitch;

void setup() {
  attachInterrupt(digitalPinToInterrupt(Mswitch),readSwitch,RISING);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(switch_pin,INPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
  if (MasterInit == 1){    //switch stays high  //This is MASTER Statement
    Wire.end();  
    Wire.begin(); 
    Serial.begin(9600);
                                                                        //Potential issue: Does the setup function only get called once??
  }   // start serial for output
  else if(MasterInit == 0){             //This is SLave Statement
    Wire.end();
    Wire.begin(TEAM1);
    Serial.begin(9600); 
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    }   // start serial for output
}

void receiveEvent(int){
  int MasterSend1 = Wire.read();       //reads handover byte from master1
 // int MasterSend2 = Wire.read();   //remember to use this to match up to protocol
  if (MasterSend1 == 3 ){
    Serial.println(3);
    MasterInit = 1;
    Serial.println("Recieved");
    setup(); 
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

void readSwitch() {
  int sVal = digitalRead(Mswitch);
  if (sVal == 1) {
    MASTERswitch = 1;
  }
  else{
    MASTERswitch = 0;
  }
}

void loop() {
  //TEAM1 IS MASTER
  if (MasterInit==1){             //code to execute when master
    Serial.println("Master! ");            
    if (MASTERswitch == 0) {        //Switch off, waiting for switch to run and send data
      digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);  //Board LED goes low  
      delay(500);
      Serial.println("Master T1 Wait ");
      setup(); 
    }
    else if (MASTERswitch == 1){                //master, sending data
      Serial.println("Done!");
      digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high for Master operation

      //ENTER YOUR TEAMS CODE HERE

      //ENTER YOUR TEAMS CODE HERE

      delay(5000);
      Wire.beginTransmission(Z_MOTION); 
      Wire.write(Z_Offset);        
      Wire.endTransmission(); 
      delay(500);
    
      //Master Handover to Team 2
      delay(10000);
      Wire.beginTransmission(TEAM2);  //This is sending handover to team3, Master2 writing to slave3
      Wire.write(MT1);        
      Wire.endTransmission(); 
      delay(100);
      Serial.println("GOOD: Team1 sends Master too team 2 ");
      Serial.println("Team1 sends byte to Team2 ");
      delay(1000);
      MasterInit = 0;
      setup();                                      //only need to switch high to make master, otherwise it is slave 
    }
  }
  // TEAM 1 IS SLAVE
  else if (MasterInit == 0) {         
    delay(100); 
    if (MASTERswitch == 1){                
      Serial.println("Slave 1 switch on ");
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);  //Board LED goes low  
      delay(250);
      setup(); 
    }
    else if (MASTERswitch == 0) {
      Serial.println("Team1 Slave ");
      delay(100);
      setup();
    }
  }
}
