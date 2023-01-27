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
  switch_val = digitalRead(switch_pin);
  if ((switch_val==HIGH) && (digitalSLAVE==0)){     //MASTER SETUP
    Wire.begin(); 
    Serial.begin(9600); 
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
  }   // start serial for output
  else if ((switch_val==LOW) && (digitalSLAVE==1)) {     //SLAVE SETUP    //Potential issue: Does the setup function only get called once??
    Wire.begin(TEAM1);                                    //Could look into serial.available for acknowledgment, but this should be inside loop function
    Serial.begin(9600);
    Wire.onReceive(receiveEvent);
    }   // start serial for output

}

void receiveEvent(int howmany){
  int Data = Wire.read();          // receive byte as an integer
  Serial.println(Data);            // print the integer
  digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);  //Board LED goes high
        //need to send acknowledgement bit to team 2
  Wire.write(1);  //Writes hexadecimal byte (or maybe just int 1) to master 
}

void loop() {
  switch_val = digitalRead(switch_pin);
  //Main block of TEAM1 Master Code
  if ((switch_val==HIGH) && (digitalSLAVE==0)){              //code to execute when master
    delay(1000);
    Wire.beginTransmission(Z_MOTION);  //This is Z-controller info
    Wire.write(x);        
    Wire.endTransmission(); 
    delay(500);
    //Master handover to TEAM2
    delay(1000);
    Wire.beginTransmission(TEAM2);  //This is sending handover to team2, Master1 writing to slave2
    Wire.write(y);        
    Wire.endTransmission(); 
    digitalSLAVE = 1;
    delay(500);
    setup();
  }
  else if ((switch_val==LOW) && (digitalSLAVE==1)){
    delay(100);        //slave stuff
  }
}
