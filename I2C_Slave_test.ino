//----------------------------------------------------slave code

// Wire Peripheral Sender
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Sends data as an I2C/TWI peripheral device
// Refer to the "Wire Master Reader" example for use with this

// Created 29 March 2006

// This example code is in the public domain.

#include "hardware/i2c.h"
#include <Wire.h>

void setup() {
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write("hello "); // respond with message of 6 bytes
  // as expected by master
}