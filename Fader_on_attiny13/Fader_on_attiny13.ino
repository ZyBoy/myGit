/*
Моргалка на attiny13
*/

int led[] = {0,1,2,3,4,5};           // the pin that the LED is attached to
int brightness[] = {0,10,50,100,150,200};    // how bright the LED is
int fadeAmount[] = {5,5,5,5,5,5};    // how many points to fade the LED by

// the setup routine runs once when you press reset:
void setup() {
  // declare pins to be an output:
   for (byte i=0;i<6;i++) {
     pinMode(led[i], OUTPUT);
   }
}

// the loop routine runs over and over again forever:
void loop() {

  for (byte i=0;i<6;i++) {
    // set the brightness of pin 9:
    analogWrite(led[i], brightness[i]);

    // change the brightness for next time through the loop:
    brightness[i] = brightness[i] + fadeAmount[i];

    // reverse the direction of the fading at the ends of the fade:
    if (brightness[i] == 0 || brightness[i] == 255) {
      fadeAmount[i] = -fadeAmount[i] ;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}

