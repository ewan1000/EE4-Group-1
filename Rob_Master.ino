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
  Serial.print("Writing message:\n");

  char buf[sizeof(header)];    //10=w
  memcpy(buf, &header, sizeof(header));

  int coord_num = 5;
  int coord_arr[] = {240,1800,100,500,3200};            
  char buf_xarr[sizeof(coord_arr)];          // buffer to send with null terminator
 // memcpy(buf_x, &x, sizeof(x));       // encode value to buffer
  Serial.println(sizeof(coord_arr));   
  int w = Wire.write(buf, sizeof(buf));    
  for (int i=0;i<coord_num;i++)
    {
     /* Serial.println(coord_arr[i]);
      memcpy(&buf_xarr[i*4], &coord_arr[i], sizeof(coord_arr[i]));
      Wire.write(&buf_xarr[i],sizeof(buf_xarr[i*4]));       // encode value to buffer
      delay(20);*/
      //INSTEAD OF DOING ABOVE, COULD TRY RECREATING BUF EACH TIME AND SENDING OVER
      //INT FROM COORD_ARR, DELAY ENOUGH SO THAT IT IS RECEIVED BY SLAVE
      //ie what's below
      Serial.println(coord_arr[i]);
      char buf_x[sizeof(coord_arr[i])];
      memcpy(buf_x, &coord_arr[i], sizeof(coord_arr[i])); //dst, src, no of bytes
      Wire.write(buf_x, sizeof(buf_x));       // write address to read from, write specified number of bytes
      delay(100);


    }    //creating variable name may not work as concatenating string, also above line may be difficult to declare iteratively ie making diff name for buf_x
         //COULD TRY PUTTING ALL COORDS IN BUF_X, BUT OFFSETTING SO THAT THEY DONT OVERWRITE EACH OTHER

 // Wire.write(buf_xarr,sizeof(buf_xarr));
  //Wire.write(buf_y,sizeof(buf_y));
  Serial.print("Wrote ");
  Serial.print(w);
  Serial.print(" bytes");

  Serial.println("Terminating transmission...");
  Wire.endTransmission();
  Serial.println("Transmission terminated.");

  delay(2000);
}
