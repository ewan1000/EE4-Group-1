

//----------------------------------------------------master code
// Wire Controller Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI peripheral device
// Refer to the "Wire Peripheral Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include "hardware/i2c.h"
#include <Wire.h>


//#define I2C_PORT i2c0
#define SLAVE_ADDR 8

void setup() {
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

void loop() {
  Wire.requestFrom(8, 6);    // request 6 bytes from peripheral device #8
  
  while (Wire.available()) { // peripheral may send less than requested
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);
}