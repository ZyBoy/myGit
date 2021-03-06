byte i1 = 0;
byte i2 = 0;

void setup() {
   for (byte i=0;i<5;i++) {
     pinMode(i, OUTPUT);
   }
}

void loop() {
  byte i2 = random(0, 5); // все же (0,5), а не (0,4)

  // вдруг повезет
  //i1 = millis()%5; // вместо рандома получаем время с момента запуска и берем остаток от деления на 5 (значения от 0 до 4)
  //i1 = ((millis()%1000)^3)%5; // А если так? берем последние 3 цифры, возводим в куб и берем "наш остаток" {0,1,2,3,4}

  if (i1 != i2){ // Что бы не было когда горит один диод, потому что рандом 2 раза одно и то же выдал.
    digitalWrite(i2,HIGH); // Зажигаем "второй" диод (первый типа уже горит, по факту один гореть будет только при старте и при "совпадении рандома")
      delay(7);            // пауза (горят два диода)
    digitalWrite(i1,LOW);  // Тушим первый диод (который горит уже 2 такта)

    i1 = i2;               // Переопределяем диоды, т.к. горит один только, он становится первым.
  }

}



