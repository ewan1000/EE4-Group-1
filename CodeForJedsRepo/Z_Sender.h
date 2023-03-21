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
    Serial.println(z_coord);
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
