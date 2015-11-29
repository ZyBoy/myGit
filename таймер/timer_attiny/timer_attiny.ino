/*
 Программа поочередно включается все светодиоды, подключенные к двум
сдвиговым регистрам 74HC595 .Created 22 May 2009
 Modified 23 Mar 2010
 by Tom Igoe
 */

//Пин подключен к ST_CP входу 74HC595
const int latchPin = 0;
//Пин подключен к SH_CP входу 74HC595
const int clockPin = 1;
//Пин подключен к DS входу 74HC595
const int dataPin = 2;

char inputString[2];

void setup() {
   //устанавливаем режим OUTPUT
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
}

void loop() {
  // проходим циклом по всем 16 выходам двух регистров
  for (int thisLed = 0; thisLed < 16; thisLed++) {
    // записываем сигнал в регистр для очередного светодиода
    registerWrite(thisLed, HIGH);
    // если это не первый светодиод, то отключаем предыдущий
    if (thisLed > 0) {
      registerWrite(thisLed - 1, LOW);
    } 
    // если это первый светодиод, то отключаем последний
    else {
      registerWrite(15, LOW);
    } 
    // делаем паузу перед следующией итерацией
    delay(250);
  }

}

// этот метод отсылает бит на сдвиговый регистр

void registerWrite(int whichPin, int whichState) {
  // для хранения 16 битов используем unsigned int
  unsigned int bitsToSend = 0;    

  // выключаем светодиоды на время передачи битов
  digitalWrite(latchPin, LOW);

  // устанавливаем HIGH в соответствующий бит 
  bitWrite(bitsToSend, whichPin, whichState);

  // разбиваем наши 16 бит на два байта 
  // для записи в первый и второй регистр
  byte registerOne = highByte(bitsToSend);
  byte registerTwo = lowByte(bitsToSend);

  // "проталкиваем" байты в регистры
  shiftOut(dataPin, clockPin, MSBFIRST, registerTwo);
  shiftOut(dataPin, clockPin, MSBFIRST, registerOne);

  // "защелкиваем" регистр, чтобы биты появились на выходах регистра
  digitalWrite(latchPin, HIGH);
}
