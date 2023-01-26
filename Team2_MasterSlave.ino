//--------MASTER code

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define SLAVE_ADDR 8
#define TEAM2 52


byte x = 10;
bool switch;
byte PACKET = 0x01;
int MasterInit=0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5); 
  if (switch==HIGH and MasterInit==1){    //switch stays high  //This is MASTER Statement
    Wire.begin(); 
    Serial.begin(9600);
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
    
  }   // start serial for output
  else if (switch==LOW and MasterInit==0) {             //This is SLave Statement
    Wire.begin(TEAM2);
    Serial.begin(9600); 
    Wire.onReceive(receiveEvent);
    }   // start serial for output

}

void receiveEvent(){
  int MasterSend = Wire.read();
  if (MasterSend == PACKET){
    MasterINIT = 1
  else {
    MasterINIT = 0
      }
  } 

}

void loop() {
  delay(1000);
  Wire.beginTransmission(SLAVE_ADDR); 
  Wire.write(x);        
  Wire.endTransmission(); 
  delay(500);
}