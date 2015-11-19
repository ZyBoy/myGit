void setup() {
   for (byte i=0;i<5;i++) {
     pinMode(i, OUTPUT);
   }
}

void loop() {
  //byte i = random(0, 5);

  // вдруг повезет
  byte i = millis()%6; // вместо рандома получаем время с момента запуска и берем остаток от деления на 6 (значения от 0 до 5)

  digitalWrite(i,HIGH);
    delay(15);
  digitalWrite(i,LOW);
    delay(1);
}



