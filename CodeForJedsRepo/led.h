#ifndef LED_H_
#define LED_H_

#include <Arduino.h>

// reset all LEDs
void resetLEDs(int isoG, int isoY, int isoR, int contG, int contY, int contR)
{
    digitalWrite(isoG, LOW); 
    digitalWrite(isoY, LOW); 
    digitalWrite(isoR, LOW); 
    digitalWrite(contG, LOW); 
    digitalWrite(contY, LOW);  
    digitalWrite(contR, LOW); 
} 

void testLEDs(int commG, int commR, int isoG, int isoY, int isoR, int contG, int contY, int contR)
{
    digitalWrite(commG, HIGH);
    digitalWrite(commR, HIGH);
    digitalWrite(isoG, HIGH); 
    digitalWrite(isoY, HIGH); 
    digitalWrite(isoR, HIGH); 
    digitalWrite(contG, HIGH); 
    digitalWrite(contY, HIGH);  
    digitalWrite(contR, HIGH); 

    delay(2000);

    digitalWrite(commG, LOW);
    digitalWrite(commR, LOW);
    digitalWrite(isoG, LOW); 
    digitalWrite(isoY, LOW); 
    digitalWrite(isoR, LOW); 
    digitalWrite(contG, LOW); 
    digitalWrite(contY, LOW);  
    digitalWrite(contR, LOW); 
} 

void flashLED(int LED){
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(100);

}

#endif