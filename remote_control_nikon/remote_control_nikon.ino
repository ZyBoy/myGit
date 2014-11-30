#include "U8glib.h"
#include "utility\my5x7rus.h"

int outPin = 12; // IR-светодиод
int inPinOk = 3;  // кнопка [Ok]
int inPinUp = 4;  // кнопка [▲]
int inPinDw = 5;  // кнопка [▼]
int inPinLf = 6;  // кнопка [◄]
int inPinRt = 7;  // кнопка [►]

// инициализация дисплея
U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8


void setup() {
  u8g.setRot180(); // Перевернем экран на 180 градусов

  pinMode(outPin, OUTPUT);   // зададим режим порта OUTPUT
  digitalWrite(outPin, LOW); // диод по умолчанию погашен

  pinMode(inPinOk, INPUT);    // зададим режим порта INPUT
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


void loop() {
  // put your main code here, to run repeatedly:
  shot();
  delay(2000);        // pauses








}
