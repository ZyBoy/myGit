#include "U8glib.h"
#include "utility\my5x7rus.h"


U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8

void draw(void) {
  u8g.setFont(my5x7rus);
  u8g.drawStr(0,6,"Пример вывода русского шрифта");
  u8g.drawStr(0,14,"через библиотеку U8G");

}

void setup(void) {
  
  }

void loop(void) {
  // picture loop
  u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
  
    delay(500);
}
