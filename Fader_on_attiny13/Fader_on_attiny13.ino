void setup() {
   for (byte i=0;i<5;i++) {
     pinMode(i, OUTPUT);
   }
}

void loop() {
  byte i = random(0, 5);

  digitalWrite(i,HIGH);
    delay(15);
  digitalWrite(i,LOW);
    delay(1);
}



