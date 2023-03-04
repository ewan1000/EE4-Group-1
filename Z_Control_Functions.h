#include "hardware/i2c.h"
#include <Wire.h>
#include <AccelStepper.h>
#include <math.h>

#define I2C_PORT i2c0
#define TEAM1 51
#define TEAM2 52
#define TEAM3 53
#define TEAM4 54
#define XY_MOTION 55
#define Z_MOTION 56

void Z_Coord_Sender(int z_coord)
  {
    //beginning transmission to z_slave
    Wire.beginTransmission(Z_MOTION);  
    char header = 'a';     
    char buf[sizeof(header)];  
    memcpy(buf, &header, sizeof(header));  //destination, source, no. of bytes
    Wire.write(buf, sizeof(buf));      //writing header message to Z_motion, imitating protocol 

    //Master Sends coord to slave
    Serial.println("Displacement[um]:\t");       //check printing correct coord
    Serial.print(z_coord);
    char buf_z[sizeof(z_coord)];   //make char buffer size of integer, this buffer is overwritten each for loop iteration
    memcpy(buf_z, &z_coord, sizeof(z_coord)); //dst, src, no of bytes
    Wire.write(buf_z, sizeof(buf_z));       // write address to read from, write specified number of bytes
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
  {   //MAX 63000 um, team_offset varies for each team, depending on head
    Wire.beginTransmission(Z_MOTION);  
    char header = 'a';     
    char buf[sizeof(header)];  
    memcpy(buf, &header, sizeof(header));  //destination, source, no. of bytes
    Wire.write(buf, sizeof(buf));      //writing header message to Z_motion, imitating protocol 

    Serial.print("Team Specific Z-offset:"); 
    Serial.println(team_offset);
    char buf_z[sizeof(team_offset)];   //make char buffer size of integer, this buffer is overwritten each for loop iteration
    memcpy(buf_z, &team_offset, sizeof(team_offset)); //dst, src, no of bytes
    Wire.write(buf_z, sizeof(buf_z));       // write address to read from, write specified number of bytes
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

    int z_zeroing_val = -40000;  
    Serial.print("Zeroing value:\t"); 
    Serial.println( z_zeroing_val);
    char buf_z[sizeof(z_zeroing_val)];   //make char buffer size of integer, this buffer is overwritten each for loop iteration
    memcpy(buf_z, &z_zeroing_val, sizeof(z_zeroing_val)); //dst, src, no of bytes
    Wire.write(buf_z, sizeof(buf_z));       // write address to read from, write specified number of bytes
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