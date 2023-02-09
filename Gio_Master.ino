#include <Arduino.h>
//#include <Serial.h>
#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("ARM SEGMENT MOCK-UP");
  Serial.println("initialising I2C (Wire)...");
  Serial.println("mode:\tmaster");
  Wire.begin();
  Serial.println("I2C initialised!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Initialising transmission with slave device at 0xbf...");
  Wire.beginTransmission(0xbf);

  Serial.println("Transmission initialised!");
  char header = 'a';
  float x = 123.456;            
  int y = 3200;    // value to encode
  Serial.print("Writing message:\n");
  
  Serial.println(x);
  char buf_x[sizeof(x)];          // buffer to send with null terminator
  memcpy(buf_x, &x, sizeof(x));       // encode value to buffer

  Serial.println(y);
  char buf_y[sizeof(y)];
  memcpy(buf_y, &y, sizeof(y));
  
  char buf[sizeof(header)];    //10=w
  memcpy(buf, &header, sizeof(header));
 // int acc = 0;
 // strcpy(buf, header);
 // acc += sizeof(header);
  //strcpy(&buf[acc], buf_x);
  //acc += sizeof(buf_x);
 // strcpy(&buf[acc], buf_y);
 // acc += sizeof(buf_y);
  int w = Wire.write(buf, sizeof(buf));
  Wire.write(buf_x,sizeof(buf_x));
  Wire.write(buf_y,sizeof(buf_y));
  Serial.print("Wrote ");
  Serial.print(w);
  Serial.println(" bytes");

  Serial.println("Terminating transmission...");
  Wire.endTransmission();
  Serial.println("Transmission terminated.");

  delay(2000);
}
