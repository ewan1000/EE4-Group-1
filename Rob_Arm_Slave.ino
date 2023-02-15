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

  char header = buf[0];            //need to know byte size of types
  Serial.print("header:\t");         //single letter is one byte
  Serial.println(header);

  int x1;                          // variable to decode to
  memcpy(&x1, &buf[1], sizeof(x1));       //int is 4 bytes
  Serial.print("x1:\t");
  Serial.println(x1);                // print the message

  int x2;
  memcpy(&x2, &buf[5], sizeof(x2));
  Serial.print("x2:\t");
  Serial.println(x2);

  int x3;
  memcpy(&x3, &buf[9], sizeof(x1));
  Serial.print("x3:\t");
  Serial.println(x3);

  int x4;
  memcpy(&x4, &buf[13], sizeof(int));
  Serial.print("x4:\t");
  Serial.println(x4);

  int x5;
  memcpy(&x5, &buf[17], sizeof(int));
  Serial.print("x5:\t");
  Serial.println(x5);
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