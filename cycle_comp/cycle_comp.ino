#include "U8glib.h"
#include "utility\my5x7rus.h"

int i = 1;

U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8

void draw(void) {
  String stringVar = String(i); // объявим Sring и конвертируем i в нее
  char charVar[sizeof(stringVar)];  // объявим char* размером как наша переменная Sring
  stringVar.toCharArray(charVar, sizeof(charVar)); // Преобразуем (неинтуиивная запись) String в char* (я хз какая разница между char и char*, но это критично!!!)

  u8g.setFont(my5x7rus);
  u8g.setScale2x2();
  u8g.drawStr(12,8,"км/ч");
  u8g.drawStr(1,8,charVar);
  u8g.undoScale();
}

void setup(void) {



}

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );

  delay(50);

  i = i + 1;

}

