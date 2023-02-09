#include <Arduino.h>
//#include <Serial.h>
#include <Wire.h>

void receiveEvent(int howMany)
{
  // function that executes whenever data is received from master
  // this function is registered as an event, see setup()
  Serial.println("Transmission from master device detected!");
  Serial.print("Reading message...\t");

  char buf[howMany];
  Serial.print(howMany);
  Serial.println(" bytes");
  for (int i=0; i<=howMany; i++)    // loop through all bytes received
  {
    buf[i] = Wire.read();           // append byte to buffer
  }
  Serial.print("Message received:\n");

  char header = buf[0];
  Serial.print("header:\t");
  Serial.println(header);

  float x;                          // variable to decode to
  memcpy(&x, &buf[1], sizeof(x));       // decode buffer to variable
  Serial.print("x:\t");
  Serial.println(x);                // print the message

  int y;
  memcpy(&y, &buf[1+sizeof(y)], sizeof(y));
  Serial.print("y:\t");
  Serial.println(y);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  Serial.println("T5 XY CONTROLLER");
  Serial.println("initialising I2C (Wire)...");
  Serial.println("mode:\tslave");
  Serial.println("addr:\t0xbf");
  Wire.begin(0xbf);
  Wire.onReceive(receiveEvent);
  Serial.println("I2C initialised!");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(20);
}