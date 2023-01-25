//--------MASTER code

#include "hardware/i2c.h"
#include <Wire.h>

#define I2C_PORT i2c0
#define SLAVE_ADDR 8


byte x = 10;

void setup() {
  gpio_set_function(4, GPIO_FUNC_I2C);
  gpio_set_function(5, GPIO_FUNC_I2C);
  gpio_pull_up(4);
  gpio_pull_up(5);
  Wire.begin(); 
  Serial.begin(9600);  // start serial for output
}

void loop() {
  delay(1000);
  Wire.beginTransmission(SLAVE_ADDR); 
  Wire.write(x);        
  Wire.endTransmission(); 
  delay(500);
}