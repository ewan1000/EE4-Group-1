//--------MASTER/SLAVE code for TEAM 1

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define Z_MOTION 56
#define TEAM1 51
#define TEAM2 52

byte x = 10;

byte y = 0x01;
int SLAVE_INIT;
int digitalSLAVE=0;
int switch_pin = 6;
int switch_val;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(switch_pin,INPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
  if (switch_val==HIGH and digitalSLAVE=0){    //switch stays high //MASTER SETUP
    Wire.begin(); 
    Serial.begin(9600); 
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
    Wire.write(); //z controller info 

  }   // start serial for output
  else if (switch_val==LOW and digitalSLAVE=1) {     //SLAVE SETUP
    Wire.begin(TEAM1); 
    Serial.begin(9600);
    Wire.onReceive(receiveEvent);
    SLAVE_INIT = 1;   //what is this used for??

    }   // start serial for output

}

void receiveEvent(){

      //need to send acknowledgement bit to team 2
  Wire.write(1);  //Writes hexadecimal byte (or maybe just int 1) to master  


  int Data = Wire.read();          // receive byte as an integer
  Serial.println(Data);            // print the integer
  digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
  digitalWrite(dirPin, HIGH);

}

void loop() {
  switch_val = digitalRead(switch_pin);
  //Main block of TEAM1 Master Code
  delay(1000);
  Wire.beginTransmission(Z_MOTION);  //This is Z-controller info
  Wire.write(x);        
  Wire.endTransmission(); 
  delay(500);
  //Master handover to TEAM2
  delay(1000);
  Wire.beginTransmission(TEAM2);  //This is sending handover to team2
  Wire.write(y);        
  Wire.endTransmission(); 
  digitalSLAVE = 1;
  delay(500);

}