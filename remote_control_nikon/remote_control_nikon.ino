#include "U8glib.h"
#include "utility\my5x7rus.h"

int outPin = 12; // IR-светодиод
// int inPinOk = 3;  // кнопка [Ok]
// int inPinUp = 4;  // кнопка [▲]
// int inPinDw = 5;  // кнопка [▼]
// int inPinLf = 6;  // кнопка [◄]
// int inPinRt = 7;  // кнопка [►]

#define NUM_KEYS 5
int  adc_key_val[NUM_KEYS] = {30, 350, 650, 750, 860};


// инициализация дисплея
U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8


void setup() {
  u8g.setRot180(); // Перевернем экран на 180 градусов

  pinMode(outPin, OUTPUT);   // зададим режим порта OUTPUT
  digitalWrite(outPin, LOW); // диод по умолчанию погашен

 // pinMode(inPinOk, INPUT);    // зададим режим порта INPUT
}

// Функция которая выдает кодированную команду для фотоаппарата
void shot(void) {
  digitalWrite(outPin, HIGH);   // sets the pin on
  delayMicroseconds(2000);        // pauses
  digitalWrite(outPin, LOW);    // sets the pin off
  delay(28);        // pauses

  digitalWrite(outPin, HIGH);   // sets the pin on
  delayMicroseconds(400);        // pauses
  digitalWrite(outPin, LOW);    // sets the pin off
  delayMicroseconds(1580);        // pauses

  digitalWrite(outPin, HIGH);   // sets the pin on
  delayMicroseconds(400);        // pauses
  digitalWrite(outPin, LOW);    // sets the pin off
  delayMicroseconds(3580);        // pauses

  digitalWrite(outPin, HIGH);   // sets the pin on
  delayMicroseconds(400);        // pauses

  digitalWrite(outPin, LOW);
}

// Определение нажатой кнопки
int get_key(int key_pin)
{
  int input = analogRead(key_pin);
  int k;
  for(k = 0; k < NUM_KEYS; k++) 
    if(input < adc_key_val[k]) 
      return k + 1;     
  return 0;
}


void loop() {
  u8g.firstPage();  

  int key = get_key(A0);

  String stringVar = String(key); // объявим Sring и конвертируем i в нее
  char charVar[sizeof(stringVar)];  // объявим char* размером как наша переменная Sring
  stringVar.toCharArray(charVar, sizeof(charVar)); // Преобразуем (неинтуиивная запись) String в char* (я хз какая разница между char и char*, но это критично!!!)
      do {
           u8g.setFont(my5x7rus);
           u8g.setScale2x2();
           u8g.drawStr(1,8,charVar);
           u8g.undoScale();
      } 
      while( u8g.nextPage() );


  if (key == 2)
    {
    // put your main code here, to run repeatedly:
    shot();
    delay(2000);        // pauses
    }










}
