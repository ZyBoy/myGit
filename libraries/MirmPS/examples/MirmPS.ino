#include <MirmPS.h>

void setup() 
  {
    Serial.begin(9600);
    TIMSK2=1; // Обязательно для многозадачности!!!
  }

void loop2() 
  {
    digitalWrite(13,1);
    delay(500);
    digitalWrite(13,0);
    delay(500);
  }

void loop1()
  {
    Serial.println("HelloWorld");
    delay(2000);
  }
