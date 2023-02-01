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
int Y_LED = 13;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(Y_LED, OUTPUT);
  pinMode(switch_pin,INPUT);
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
  if ((digitalRead(switch_pin)==HIGH) && (MasterInit==1)){     //MASTER SETUP
    Wire.end();
    Wire.begin(); 
    digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high
    }
  else if((digitalRead(switch_pin)==LOW) && (MasterInit==0)) {     //SLAVE SETUP    
    Wire.end();
    Wire.begin(TEAM1);                                  
    Serial.begin(9600);
    Wire.onReceive(receiveEvent);
  //  Wire.onRequest(requestEvent);
    }   // start serial for output
  else{
    Wire.end();
  }  
}
                                    //repeatedly calling setup may be causing issues, maybe add counter so setup only gets called once 
                       
void receiveEvent(int howmany){
  int MasterSend1 = Wire.read();       //reads handover byte from master1
 // int MasterSend2 = Wire.read();   //remember to use this to match up to protocol
  if (MasterSend1 == 3 ){            
    Serial.println("Remember: Switch team2 to slave ");                                    
    digitalWrite(G_LED, HIGH);  //Board LED goes HIGH
    digitalWrite(Y_LED, HIGH);  //Board LED goes LOW
    digitalWrite(R_LED, HIGH);  //Board LED goes LOW
    delay(5000);        //slave stuff
    digitalWrite(G_LED, LOW);  //Board LED goes HIGH
    digitalWrite(Y_LED, LOW);  //Board LED goes LOW
    digitalWrite(R_LED, LOW);  //Board LED goes LOW
    MasterInit = 1;    
  }
  else {            //this needs to be empty: only trigger receive LED alert if successful read, otherwise do nothing
      }  
}

void loop() {
  //Main block of TEAM1 Master Code
  if ((digitalRead(switch_pin)==LOW) && (MasterInit==0)){
    digitalWrite(G_LED, LOW);  
    digitalWrite(Y_LED, HIGH);  
    digitalWrite(R_LED, LOW);   
    Serial.println("Good: Team1 is slave "); 
    delay(1000);        //slave stuff
    setup();   //this is for changing digital slave back to 0, once team4 is finished
  }
   else if ((digitalRead(switch_pin)==LOW) && (MasterInit==1)){
    digitalWrite(G_LED, HIGH);  
    digitalWrite(Y_LED, HIGH);  
    digitalWrite(R_LED, LOW);  
    Serial.println("BAD: Switch to make Team1 Master "); 
    delay(1000);        //slave stuff
    setup();   //this is for changing digital slave back to 0, once team4 is finished
  }
   else if ((digitalRead(switch_pin)==HIGH) && (MasterInit==0)){
    digitalWrite(G_LED, LOW); 
    digitalWrite(Y_LED, HIGH);  
    digitalWrite(R_LED, HIGH); 
    Serial.println("BAD: switch to make team1 slave");  
    delay(1000);        //slave stuff
    setup();   //this is for changing digital slave back to 0, once team4 is finished
  }
  else if ((digitalRead(switch_pin)==HIGH) && (MasterInit==1)){
    digitalWrite(G_LED, HIGH);  //Board LED goes HIGH
    digitalWrite(Y_LED, LOW);  //Board LED goes LOW
    digitalWrite(R_LED, LOW);  //Board LED goes LOW
    //Master handover to TEAM2
    delay(1000);
    Wire.beginTransmission(TEAM2);  //This is sending handover to team2, Master1 writing to slave2
    Wire.write(MT1);             
    Wire.endTransmission(); 
    Serial.println("GOOD: Team1 is Master");
    Serial.println("Team1 sends byte to Team2");
    delay(1000);
    MasterInit = 0;                         //would be better if this changes when its confirmed slave has received and read the message
    setup();
   }                      
}
