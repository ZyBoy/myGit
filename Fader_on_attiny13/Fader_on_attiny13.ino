byte i1 = 0;
byte i2 = 0;

void setup() {
   for (byte i=0;i<5;i++) {
     pinMode(i, OUTPUT);
   }
}

void loop() {
  byte i1 = random(0, 5);

  // вдруг повезет
  //i1 = millis()%6; // вместо рандома получаем время с момента запуска и берем остаток от деления на 6 (значения от 0 до 5)

  digitalWrite(i1,HIGH);
    delay(7);
  digitalWrite(i2,LOW);

  i2 = i1;


}



