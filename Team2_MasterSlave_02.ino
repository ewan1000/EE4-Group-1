//--------Team 2 SLAVE/MASTER code

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define TEAM1 51
#define TEAM2 52



byte MT1 = 3;  //Master transfer 1
int MasterInit=0;
int switch_pin = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(switch_pin,INPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5); 
  if ((digitalRead(switch_pin)==HIGH) && (MasterInit==1)){    //STATE 2  //This is MASTER Statement
    Wire.end();
    Wire.begin(); 
    Serial.begin(9600);
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high                                                                   //Potential issue: Does the setup function only get called once??
  }   // start serial for output
  else if ((digitalRead(switch_pin)==LOW) && (MasterInit==0)) {             //This is SLave Statement //This is STATE 1 for team2
    Wire.end();
    Serial.println(1);
    Wire.begin(TEAM2);                                                    //MAYBE NEED TO PUT IN A WIRE.END(), POTENTIALLY NOT ENDING I2C BUS CORRECTLY
    Serial.begin(9600); 
    Wire.onReceive(receiveEvent);
    }   // start serial for output
  else {
    Wire.end();                                                //need to end the I2C connection with wire.end, otherwise it remains
    Serial.println(4);
  }  

}

void receiveEvent(int howmany){
  int MasterSend1 = Wire.read();       //reads handover byte from master1
 // int MasterSend2 = Wire.read();   //remember to use this to match up to protocol
  if (MasterSend1 == 3 ){                                               
    Serial.println(MT1);
    MasterInit = 1;  
  }
  else {
    MasterInit = 0;
      }  
}


void loop() {
  //TEAM2 IS MASTER
  if ((digitalRead(switch_pin)==HIGH) && (MasterInit==1)){              //code to execute when master
    //Master Handover to Team 3
    digitalWrite(LED_BUILTIN, LOW);  //Board LED goes high
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
    delay(1000);
    Wire.beginTransmission(TEAM1);  //This is sending handover to team2, Master1 writing to slave2
    Wire.write(MT1);             
    Wire.endTransmission(); 
    Serial.println(7);
    delay(1000);
    setup();
  }
  //TEAM2 IS SLAVE
  else if ((digitalRead(switch_pin)==LOW) && (MasterInit==0)){
    delay(100);   
    Serial.println(6);   
    delay(2000);                                                                      //setup maybe getting called too quickly
    setup();            //slave stuff
  }
   else{ 
      digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);  //Board LED goes low  
      delay(100);  
      digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);  //Board LED goes low  
      delay(100);  
      setup();
  }
}


/////Look into setup(), is this possible to be called //Setup does get called I believe
//check if the team1 is writing to team2 properly to send masterpacket //Getting issues with this now