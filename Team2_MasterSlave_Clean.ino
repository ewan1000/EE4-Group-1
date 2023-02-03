//--------Team 2 SLAVE/MASTER code

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define TEAM1 51
#define TEAM2 52
#define TEAM3 53
#define TEAM4 54
#define Z_MOTION 56

byte Z_Offset = 100;
byte MT1 = 3;  //Master transfer 1 byte, stage handover byte
//byte MT2 = 0xAA;

int R_LED_MASTER = 16;
int G_LED_SLAVE = 17;
int MASTERswitchSTATE;  //the switch only turns the MCU into a master, slave is activated automatically
int MasterInit = 0;      //Team1 is initially master 
int switch_pin = 1;  //switch is GPIO 1

void setup() {
  attachInterrupt(digitalPinToInterrupt(switch_pin),readSwitch,RISING); //Interrupt called on switch pin on logic rising from low to high
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(switch_pin,INPUT);
  pinMode(R_LED_MASTER, OUTPUT);
  pinMode(G_LED_SLAVE, OUTPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);      //sets up pull-up resistors on I2C pins, essential for I2C
  gpio_pull_up(5);
  if (MasterInit == 1){     //Master Setup: Team2 is configured to Master on I2C0 channel
    Wire.end();  
    Wire.begin(); 
    Serial.begin(9600);

  }   
  else if(MasterInit == 0){            //Slave Setup: Team2 is configured to Slave on I2C0 channel
    Wire.end();
    Wire.begin(TEAM2);
    Serial.begin(9600); 
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    }   
}

void receiveEvent(int x){   //in wire.receiveEvent definition, an int must be declared in scope
  int Slave2Receives = Wire.read();       
  //reads handover byte from master1
  if (Slave2Receives == 3 ){          //if team2 as slave receives a 3 from other team acting as master
    Serial.println(3);
    MasterInit = 1;                 //Now team2 can be initialised as master instead of slave
    Serial.println("Received Byte from Master");
    setup(); 
  }
  else {
    MasterInit = 0;
      }  
}

void requestEvent(){
  //requestEvent is for sending data received acknowledgement bit to team 3 (or any team that is acting as master)
  //Team1 sends this to team3 as a slave
  Wire.write(1);  
  //Wire.write(0xAA);     
}

void readSwitch() {
  int sVal = digitalRead(switch_pin);
  if (sVal == 1) {
    MASTERswitchSTATE = 1;
  }
  else{
    MASTERswitchSTATE = 0;
  }
}

void loop() {
  //TEAM2 has been initialised as MASTER
  if (MasterInit==1){        
    Serial.println("Master! ");            //Prints to serial monitor
    if (MASTERswitchSTATE == 0) {
      digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high for Master operation
      digitalWrite(G_LED_SLAVE, LOW);  //Slave LED goes LOW
      digitalWrite(R_LED_MASTER, LOW);  //Master state LED goes LOW 
      Serial.println("Master is stuck in the chamber");
      Serial.println("Flick Switch to change state and send byte");
      delay(2000);
      setup(); 
    }
    else if (MASTERswitchSTATE == 1){
      Serial.println("Team2 Master is ready to transmit");
      digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high for Master operation
      digitalWrite(G_LED_SLAVE, LOW);  //Slave LED goes LOW
      digitalWrite(R_LED_MASTER, HIGH);  //Master state LED goes HIGH
      //ENTER YOUR TEAMS CODE HERE
      delay(2000);
      Wire.beginTransmission(Z_MOTION);  //Example Z motion transmission 
      Wire.write(Z_Offset);        
      Wire.endTransmission(); 
      delay(500);
    
      //Master Handover to Team 3 
      delay(2000);
      Wire.beginTransmission(TEAM3);  
      Wire.write(MT1);        //WRITING MT1 BYTE = 3
      Wire.endTransmission(); 
      delay(100);
      Serial.println("GOOD: Team2 has sent MT1 byte to team 3 ");
      delay(1000);
      MasterInit = 0;
      setup();
    }
  }
  // TEAM 2 has been initialised as SLAVE
  else if (MasterInit == 0) {
    Serial.println("SLAVE! ");  
    if (MASTERswitchSTATE == 1){
      Serial.println("Team2 is now a slave ");
      digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW for Slave operation
      digitalWrite(G_LED_SLAVE, HIGH);  //Slave LED goes HIGH
      digitalWrite(R_LED_MASTER, LOW);  //Master state LED goes LOW
      delay(2000);
      setup(); 
    }
    else if (MASTERswitchSTATE == 0) {
      Serial.println("Team2 is now a slave ");
      digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW for Slave operation
      digitalWrite(G_LED_SLAVE, HIGH);  //Slave LED goes HIGH
      digitalWrite(R_LED_MASTER, LOW);  //Master state LED goes LOW
      delay(2000);
      setup();
    }
  }
}
