//--------MASTER/SLAVE code for TEAM 1

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define Z_MOTION 56
#define TEAM1 51
#define TEAM2 52

byte Z_Offset = 100;
byte MT1 = 0x03;  //Master transfer 1
byte MT2 = 0xAA;
int MasterInit=1;
int switch_pin = 6;
bool switch_val;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(switch_pin,INPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
  switch_val = digitalRead(switch_pin);
  if (MasterInit==1){     //MASTER SETUP
    Wire.begin(); 
    Serial.begin(9600); 
  }   // start serial for output
  else  {     //SLAVE SETUP    //Potential issue: Does the setup function only get called once??
    Wire.begin(TEAM1);                                    //Could look into serial.available for acknowledgment, but this should be inside loop function
    Serial.begin(9600);
    Wire.onRequest(requestEvent);
    Wire.onReceive(receiveEvent);
    }   // start serial for output
}

void requestEvent(){
        //need to send acknowledgement bit to team 2
  Wire.write(0x03);  //Writes hexadecimal byte (or maybe just int 1) to master //check byte
 // Wire.write(0xAA);     //data byte
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

void loop() {
  switch_val = digitalRead(switch_pin);
  //Main block of TEAM1 Master Code
  if (MasterInit==1) { 
    while(switch_val==LOW) {            // Master operation is holted until the head is changed and Master switch is turned on
      digitalWrite(LED_BUILTIN, HIGH);  // Board LED goes high
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);   // Board LED goes low  
      delay(500);
      Serial.print("Waiting");
    }
    Serial.print("Done!");
    digitalWrite(LED_BUILTIN, HIGH);    // Board LED goes high for Master operation

    //ENTER YOUR TEAMS CODE HERE

    //ENTER YOUR TEAMS CODE HERE

    delay(1000);
    Wire.beginTransmission(Z_MOTION);  //This is Z-controller info
    Wire.write(Z_Offset);        
    Wire.endTransmission(); 
    delay(500);
    
    
    //Master handover to TEAM2
    delay(10000);
    Wire.beginTransmission(TEAM2);  //This is sending handover to team2, Master1 writing to slave2
    Wire.write(MT1);        
   // Wire.write(MT2);     
    Wire.endTransmission(); 
    Wire.end();
    MasterInit = 0;
    delay(500);
    setup();
  }
  else if (MasterInit==0) {
    delay(100); 
    if (switch_val==HIGH){
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
      delay(100);
      Wire.end();  
      setup();
    }
  }
}
