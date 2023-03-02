//--------Team 1 SLAVE/MASTER code

#include "hardware/i2c.h"
#include <Wire.h>
#include <AccelStepper.h>


#define motorInterfaceType 1
// define pin number allocation
#define dirPin 14
#define stepPin 15
#define stepMs1Pin 10
#define stepMs2Pin 11
#define stepMs3Pin 12

#define I2C_PORT i2c0
#define TEAM1 51
#define TEAM2 52
#define TEAM3 53
#define TEAM4 54
#define XY_MOTION 55
#define Z_MOTION 56

byte Z_Offset = 100;
byte MT1 = 3;  //Master transfer 1 byte, stage handover byte
//byte MT2 = 0xAA;

int R_LED_MASTER = 17;
int G_LED_SLAVE = 16;
int MASTERswitchSTATE;  //the switch only turns the MCU into a master, slave is activated automatically
int MasterInit = 1;      //Team1 is initially master 
int switch_pin = 1;  //switch is GPIO 1

void setup()    
  {
    attachInterrupt(digitalPinToInterrupt(switch_pin),readSwitch,RISING); //Interrupt called on switch pin on logic rising from low to high
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(switch_pin,INPUT);
    pinMode(R_LED_MASTER, OUTPUT);
    pinMode(G_LED_SLAVE, OUTPUT);
    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);
    gpio_pull_up(4);      //sets up pull-up resistors on I2C pins, essential for I2C
    gpio_pull_up(5);
    if (MasterInit == 1)
      {     //Master Setup: Team1 is configured to Master on I2C0 channel
        Wire.end();  
        Wire.begin(); 
        Serial.begin(9600);
      }   
    else if(MasterInit == 0)
      {            //Slave Setup: Team1 is configured to Slave on I2C0 channel
        Wire.end();
        Wire.begin(TEAM1);
        Serial.begin(9600); 
        Wire.onReceive(receiveEvent);
        Wire.onRequest(requestEvent);
      }   
  }

void receiveEvent(int howMany)
  {   // howMANY is always equal to no. of bytes received
    int Slave1Receives = Wire.read();       
    //reads handover byte from master4
    if (Slave1Receives == 3 )
      {          //if team1 as slave receives a 3 from other team acting as master
      Serial.println(3);
      MasterInit = 1;                 //Now team1 can be initialised as master instead of slave
      Serial.println("Received Byte from Master");
      setup(); 
      }
    else 
      {
      MasterInit = 0;
      }  
  }


void readSwitch()  
  {
    int sVal = digitalRead(switch_pin);
    if (sVal == 1) 
      {
        MASTERswitchSTATE = 1;        
      }
    else
      {
        MASTERswitchSTATE = 0;
      }
  } 



void Z_Coord_Sender(int z_coord)
  {
    //beginning transmission to z_slave
    Wire.beginTransmission(Z_MOTION);  

    char header = 'a';     
    char buf[sizeof(header)];  
    memcpy(buf, &header, sizeof(header));  //destination, source, no. of bytes
    Wire.write(buf, sizeof(buf));      //writing header message to Z_motion, imitating protocol 

    //Master Sends coord to slave
    Serial.println("Displacement:" z_coord);       //check printing correct coord
    int z_stepVal = Displacement_Step_Converter_Z(z_coord);
    Serial.println("Motor Steps:" z_stepVal); 
    char buf_z[sizeof(z_stepVal)];   //make char buffer size of integer, this buffer is overwritten each for loop iteration
    memcpy(buf_z, &z_stepVal, sizeof(z_stepVal)); //dst, src, no of bytes
    Wire.write(buf_z, sizeof(buf_z));       // write address to read from, write specified number of bytes

    delay(100);  //delay for reception before ending transmission
    Wire.endTransmission();
      //Master requests from slave, message will be sent once motor stops moving
    int Z_Ready = 0;
    while (Z_Ready != 1)
      { //maybe add delay here to give motor time to turn away from current position 0
        Wire.requestFrom(Z_MOTION,1); //requesting 1 byte from Z pcb
        Z_Ready = Wire.read();    //Z-axis mcu sends 1 (the number) when motor is no longer turning
        Serial.print("Z_Ready:\t");
        Serial.println(Z_Ready);
        delay(100);
      }
    Z_Ready = 0;  
      //Master now needs to do individual job
    Serial.println("Motor has stopped moving, now ready to do something at this position");
    
  }  

    
int Displacement_Step_Converter_XY(int displacement) //function returns the step value after conversion
  {
    int pinion_circumference = ; //this is been calculated by hand
    int StepValue = 3200*displacement/(pinion_circumference);
    return StepValue;
  }

int Displacement_Step_Converter_Z(int displacement) //function returns the step value after conversion
  {
    int pinion_circumference = 0.09739; //this is been calculated by hand
    int StepValue = 3200*displacement/(pinion_circumference);
    return StepValue;
  }

void XYZ_Coord_Sender (int x_coord, int y_coord, int z_coord) //move xy first, then z. This is called after z_offset
  {
    //beginning transmission to xy_slave
    Wire.beginTransmission(XY_MOTION);  

    char header = 'a';     
    char buf[sizeof(header)];  
    memcpy(buf, &header, sizeof(header));  //destination, source, no. of bytes
    Wire.write(buf, sizeof(buf));      //writing header message to Z_motion, imitating protocol 

    //Master Sends X coord to  XY slave
    Serial.println("Displacement:" x_coord);       //check printing correct coord
    int x_stepVal = Displacement_Step_Converter_XY(x_coord);
    Serial.println("Motor Steps:" x_stepVal);       //check printing correct coord 
    char buf_x[sizeof(x_stepVal)];   //make char buffer size of integer, this buffer is overwritten each for loop iteration
    memcpy(buf_x, &x_stepVal, sizeof(x_stepVal)); //dst, src, no of bytes
    Wire.write(buf_x, sizeof(buf_x));       // write address to read from, write specified number of bytes

    //Master Sends Y coord to  XY slave
    Serial.println("Displacement:" y_coord);       //check printing correct coord
    int y_stepVal = Displacement_Step_Converter_XY(y_coord);
    Serial.println("Motor Steps:" y_stepVal);       //check printing correct coord 
    char buf_y[sizeof(y_stepVal)];   //make char buffer size of integer, this buffer is overwritten each for loop iteration
    memcpy(buf_y, &y_stepVal, sizeof(y_stepVal)); //dst, src, no of bytes
    Wire.write(buf_y, sizeof(buf_y));       // write address to read from, write specified number of bytes

    delay(100);  //delay for reception before ending transmission
    Wire.endTransmission();
      //Master requests from slave, message will be sent once motor stops moving
    int XY_Ready = 0;
    while (XY_Ready != 1)
      { //maybe add delay here to give motor time to turn away from current position 0
        Wire.requestFrom(XY_MOTION,1); //requesting 1 byte from Z pcb
        XY_Ready = Wire.read();    //XY-axis mcu sends 1 (the number) when motor is no longer turning
        Serial.print("XY_Ready:\t");
        Serial.println(XY_Ready);
      }
    XY_Ready = 0;  
      //Master now needs to do individual job
    Serial.println("Motors have stopped moving, now ready to do something at this position");

    //BEGINNING TRANSMISSION TO Z_SLAVE
    Wire.beginTransmission(Z_MOTION);  

    char header = 'a';     
    char buf[sizeof(header)];  
    memcpy(buf, &header, sizeof(header));  //destination, source, no. of bytes
    Wire.write(buf, sizeof(buf));      //writing header message to Z_motion, imitating protocol 

    //Master Sends coord to slave
    Serial.println("Displacement:" z_coord);       //check printing correct coord
    int z_stepVal = Displacement_Step_Converter_Z(z_coord);
    Serial.println("Motor Steps:" z_stepVal); 
    char buf_z[sizeof(z_stepVal)];   //make char buffer size of integer, this buffer is overwritten each for loop iteration
    memcpy(buf_z, &z_stepVal, sizeof(z_stepVal)); //dst, src, no of bytes
    Wire.write(buf_z, sizeof(buf_z));       // write address to read from, write specified number of bytes

    delay(100);  //delay for reception before ending transmission
    Wire.endTransmission();
      //Master requests from slave, message will be sent once motor stops moving
    int Z_Ready = 0;
    while (Z_Ready != 1)
      { //maybe add delay here to give motor time to turn away from current position 0
        Wire.requestFrom(Z_MOTION,1); //requesting 1 byte from Z pcb
        Z_Ready = Wire.read();    //Z-axis mcu sends 1 (the number) when motor is no longer turning
        Serial.print("Z_Ready:\t");
        Serial.println(Z_Ready);
        delay(100);
      }
    Z_Ready = 0;  
      //Master now needs to do individual job
    Serial.println("Motor has stopped moving, now ready to do something at this position");
    
  }


void z_offset(int team_offset) //this function is called first, this moves rack down to appropriate height so that the rack doesnt need to move back to homepoint each time
  {   //team_offset varies for each team, depending on head
    Wire.beginTransmission(Z_MOTION);  

    char header = 'a';     
    char buf[sizeof(header)];  
    memcpy(buf, &header, sizeof(header));  //destination, source, no. of bytes
    Wire.write(buf, sizeof(buf));      //writing header message to Z_motion, imitating protocol 

    int z_offset_stepVal = Displacement_Step_Converter_Z(team_offset);
    Serial.println("Team Specific Z-offset:" z_offset_stepVal); 
    char buf_z[sizeof(z_offset_stepVal)];   //make char buffer size of integer, this buffer is overwritten each for loop iteration
    memcpy(buf_z, &z_offset_stepVal, sizeof(z_offset_stepVal)); //dst, src, no of bytes
    Wire.write(buf_z, sizeof(buf_z));       // write address to read from, write specified number of bytes

    delay(100);  //delay for reception before ending transmission
    Wire.endTransmission();
      //Master requests from slave, message will be sent once motor stops moving
    int Z_Ready = 0;
    while (Z_Ready != 1)
      { //maybe add delay here to give motor time to turn away from current position 0
        Wire.requestFrom(Z_MOTION,1); //requesting 1 byte from Z pcb
        Z_Ready = Wire.read();    //Z-axis mcu sends 1 (the number) when motor is no longer turning
        Serial.print("Z_Ready:\t");
        Serial.println(Z_Ready);
        delay(100);
      }
    Z_Ready = 0;  
      //Now at offset
    Serial.println("Rack now at offset position");

  }


void z_zeroing() //this removes the offset and moves the rack back to the z origin
  {
    Wire.beginTransmission(Z_MOTION);  

    char header = 'a';     
    char buf[sizeof(header)];  
    memcpy(buf, &header, sizeof(header));  //destination, source, no. of bytes
    Wire.write(buf, sizeof(buf));      //writing header message to Z_motion, imitating protocol 

    int z_zeroing_val = -3200;
    Serial.print("Team Specific Z-offset:"); 
    Serial.println( z_zeroing_val);
    char buf_z[sizeof(z_zeroing_val)];   //make char buffer size of integer, this buffer is overwritten each for loop iteration
    memcpy(buf_z, &z_zeroing_val, sizeof(z_zeroing_val)); //dst, src, no of bytes
    Wire.write(buf_z, sizeof(buf_z));       // write address to read from, write specified number of bytes

    delay(100);  //delay for reception before ending transmission
    Wire.endTransmission();
      //Master requests from slave, message will be sent once motor stops moving
    int Z_Ready = 0;
    while (Z_Ready != 1)
      { //maybe add delay here to give motor time to turn away from current position 0
        Wire.requestFrom(Z_MOTION,1); //requesting 1 byte from Z pcb
        Z_Ready = Wire.read();    //Z-axis mcu sends 1 (the number) when motor is no longer turning
        Serial.print("Z_Ready:\t");
        Serial.println(Z_Ready);
        delay(100);
      }
    Z_Ready = 0;  
      //Now at offset
    Serial.println("Rack now at offset position");

    
  }





void loop() 
  {
    //TEAM1 has been initialised as MASTER
    if (MasterInit==1)
      {        
        Serial.println("Master! ");            //Prints to serial monitor
        if (MASTERswitchSTATE == 0) 
          {
            digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high for Master operation
            digitalWrite(G_LED_SLAVE, LOW);  //Slave LED goes LOW
            digitalWrite(R_LED_MASTER, LOW);  //Master state LED goes LOW 
            Serial.println("Flick Switch to change state and send byte");
            delay(2000);
            setup(); 
          }
        else if (MASTERswitchSTATE == 1)
          {
            Serial.println("Team1 Master is ready to transmit");
            digitalWrite(LED_BUILTIN, HIGH);  //Board LED goes high for Master operation
            digitalWrite(G_LED_SLAVE, LOW);  //Slave LED goes LOW
            digitalWrite(R_LED_MASTER, HIGH);  //Master state LED goes HIGH

            int coord_arr[] = {-500,500}; 
            int coord_num = sizeof(coord_arr)/4;  //since coord always integer, can divide total number of bytes to find number of elements
            Serial.print("Writing message:\n");
            for (int i=0;i<coord_num;i++)
              {   
                Z_Coord_Sender(coord_arr[i]);

            //ENTER YOUR TEAMS CODE HERE

                Serial.println("Now let's wait to simulate doing stuff");
                delay(1000);
                Serial.println("Humdy humdy hum");
                delay(1000);
                Serial.println("Dimdy Dumdy dee");
                delay(2000);
                Serial.println("We did a thing!!");
              }


            Serial.println("Terminating transmission...");
            Serial.println("Transmission terminated.");
                
          
            //Master Handover to Team 2
            delay(2000);
            Wire.beginTransmission(TEAM2);  //This is sending handover to team2, Master1 writing to slave2
            Wire.write(MT1);        //WRITING MT1 BYTE = 3
            Wire.endTransmission(); 
            delay(100);
            Serial.println("GOOD: Team1 has sent MT1 byte to team 2 ");
            delay(1000);
            MasterInit = 0;
            MASTERswitchSTATE = 0; //this is for 2nd time around, so that switch does automatically turn on
            setup();
          }
      }
    // TEAM 1 has been initialised as SLAVE
    else if (MasterInit == 0) 
      {
        Serial.println("SLAVE! ");  
        if (MASTERswitchSTATE == 1)
          {
            Serial.println("Team1 is now a slave ");
            digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW for Slave operation
            digitalWrite(G_LED_SLAVE, HIGH);  //Slave LED goes HIGH
            digitalWrite(R_LED_MASTER, LOW);  //Master state LED goes LOW
            delay(2000);
            setup(); 
          }
        else if (MASTERswitchSTATE == 0) 
          {
            Serial.println("Team1 is now a slave ");
            digitalWrite(LED_BUILTIN, LOW);  //Board LED goes LOW for Slave operation
            digitalWrite(G_LED_SLAVE, HIGH);  //Slave LED goes HIGH
            digitalWrite(R_LED_MASTER, LOW);  //Master state LED goes LOW
            delay(2000);
            setup();
          }
      }
  } 
