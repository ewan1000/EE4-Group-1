//--------Team 2 SLAVE/MASTER code

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define SLAVE_ADDR 8
#define TEAM1 51
#define TEAM2 52
#define TEAM3 53

byte x = 255;
byte PACKET = 0x01;
int MasterInit=0;
int switch_pin=6;
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
    }   // start serial for output

}

void receiveEvent(int howmany){
   while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  } 
  int MasterSend = Wire.read();       //reads handover byte from master1
  if (MasterSend == PACKET){
    MasterInit = 1;
  }
  else {
    MasterInit = 0;
      }  
}

void loop() {
  switch_val = digitalRead(switch_pin);
  if ((switch_val==HIGH) && (MasterInit==1)){              //code to execute when master
    delay(1000);
    Wire.beginTransmission(SLAVE_ADDR); 
    Wire.write(x);        
    Wire.endTransmission(); 
    delay(500);
  }
  else if ((switch_val==LOW) && (MasterInit==0)){
    delay(100);   
    setup();            //slave stuff
  }
}
