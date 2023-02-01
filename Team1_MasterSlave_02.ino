//--------MASTER/SLAVE code for TEAM 1

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define TEAM1 51
#define TEAM2 52

byte MT1 = 3;  //Master transfer 1
int MasterInit=1;
int switch_pin = 0;
int R_LED = 15;
int G_LED = 14;
int B_LED = 13;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
  pinMode(switch_pin,INPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
  if ((digitalRead(switch_pin)==HIGH) && (MasterInit==1)){     //MASTER SETUP
    Wire.end();
    Wire.begin(); 
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
  }   // start serial for output
  else if ((digitalRead(switch_pin)==LOW) && (MasterInit==0)) {     //SLAVE SETUP    //Potential issue: Does the setup function only get called once??
    Wire.end();
    digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW
    delay(100);
    Wire.begin(TEAM1);                                    //Could look into serial.available for acknowledgment, but this should be inside loop function
    Serial.begin(9600);
    Wire.onReceive(receiveEvent);
  //  Wire.onRequest(requestEvent);
    }   // start serial for output
  else{
    Wire.end();    
    digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW
    delay(100);
  }  

}
                                    //repeatedly calling setup may be causing issues, maybe add counter so setup only gets called once 
                       
void receiveEvent(int howmany){
  int MasterSend1 = Wire.read();       //reads handover byte from master1
 // int MasterSend2 = Wire.read();   //remember to use this to match up to protocol
  if (MasterSend1 == 3 ){                                               
    digitalWrite(G_LED, HIGH);  //Board LED goes HIGH
    digitalWrite(B_LED, HIGH);  //Board LED goes LOW
    digitalWrite(R_LED, HIGH);  //Board LED goes LOW
    delay(100);        //slave stuff
    digitalWrite(G_LED, LOW);  //Board LED goes HIGH
    digitalWrite(B_LED, LOW);  //Board LED goes LOW
    digitalWrite(R_LED, LOW);  //Board LED goes LOW
    delay(100);        //slave stuff
    digitalWrite(G_LED, HIGH);  //Board LED goes HIGH
    digitalWrite(B_LED, HIGH);  //Board LED goes LOW
    digitalWrite(R_LED, HIGH);  //Board LED goes LOW
    delay(100);        //slave stuff
    digitalWrite(G_LED, LOW);  //Board LED goes HIGH
    digitalWrite(B_LED, LOW);  //Board LED goes LOW
    digitalWrite(R_LED, LOW);  //Board LED goes LOW
  }
  else {
    MasterInit = 0;
      }  
}

void loop() {
  //Main block of TEAM1 Master Code
  if ((digitalRead(switch_pin)==HIGH) && (MasterInit==1)){              //code to execute when master
    digitalWrite(G_LED, HIGH);  //Board LED goes HIGH
    digitalWrite(B_LED, LOW);  //Board LED goes LOW
    digitalWrite(R_LED, LOW);  //Board LED goes LOW
    //Master handover to TEAM2
    delay(1000);
    Wire.beginTransmission(TEAM2);  //This is sending handover to team2, Master1 writing to slave2
    Wire.write(MT1);             
    Wire.endTransmission(); 
    delay(1000);
    MasterInit = 0;
    setup();
  }
  else if ((digitalRead(switch_pin)==LOW) && (MasterInit==0)){
    digitalWrite(G_LED, LOW);  //Board LED goes LOW
    digitalWrite(B_LED, HIGH);  //Board LED goes HIGH
    digitalWrite(R_LED, LOW);  //Board LED goes LOW
    delay(100);        //slave stuff
    setup();   //this is for changing digital slave back to 0, once team4 is finished
  }
  else{ 
    digitalWrite(G_LED, LOW);  //Board LED goes LOW
    digitalWrite(B_LED, LOW);  //Board LED goes LOW
    digitalWrite(R_LED, HIGH);  //Board LED goes high
    delay(100); 
    setup();                                  
  }
}
