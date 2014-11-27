#include "U8glib.h"
#include "utility\my5x7rus.h"

int i = 1;

U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8


// ================= код для датчика ================= 
//Время калибровки датчика (10-60 сек. по даташиту)
int calibrationTime = 30;      
//Время, в которое был принят сигнал отсутствия движения(LOW)
long unsigned int lowIn;       
//Пауза, после которой движение считается оконченным
long unsigned int pause = 5000;
//Флаг. false = значит движение уже обнаружено, true - уже известно, что движения нет
boolean lockLow = true;
//Флаг. Сигнализирует о необходимости запомнить время начала отсутствия движения
boolean takeLowTime;
int pirPin = 2;    //вывод подключения PIR датчика
// =================================================== 


void draw(void) {
  String stringVar = String(i); // объявим Sring и конвертируем i в нее
  char charVar[sizeof(stringVar)];  // объявим char* размером как наша переменная Sring
  stringVar.toCharArray(charVar, sizeof(charVar)); // Преобразуем (неинтуиивная запись) String в char* (я хз какая разница между char и char*, но это критично!!!)

  u8g.setFont(my5x7rus);
  u8g.setScale2x2();
//  u8g.drawStr(12,8,"км/ч");
  u8g.drawStr(1,8,charVar);
  u8g.undoScale();
}

void setup(void) {
 
    pinMode(pirPin, INPUT);
    digitalWrite(pirPin, LOW);
    for(int i = 0; i < calibrationTime; i++)
    {
      u8g.firstPage();  
      do {
           u8g.setFont(my5x7rus);
           u8g.drawStr(1,8,"Калибруем:");

           String stringVar = String(i); // объявим Sring и конвертируем i в нее
           char charVar[sizeof(stringVar)];  // объявим char* размером как наша переменная Sring
           stringVar.toCharArray(charVar, sizeof(charVar)); // Преобразуем (неинтуиивная запись) String в char* (я хз какая разница между char и char*, но это критично!!!)

           u8g.drawStr(60,8,charVar);
           if (i < 10)
             u8g.drawStr(69,8,"из 30");
           else
             u8g.drawStr(74,8,"из 30");
      } 
      while( u8g.nextPage() );
      
      delay(1000);
    }

  u8g.firstPage();  
  do {
       u8g.setFont(my5x7rus);
       u8g.drawStr(10,30,"Готов к работе!");
  } 
  while( u8g.nextPage() );
}

void loop(void) {
  u8g.firstPage();  

  //Если обнаружено движение
  if(digitalRead(pirPin) == HIGH)
  {
    //Если еще не вывели информацию об обнаружении
    if(lockLow)
    {
      lockLow = false;     
      do {
           u8g.setFont(my5x7rus);
           u8g.setScale2x2();
           u8g.drawStr(1,8,"Палево!");
           u8g.undoScale();
      } 
      while( u8g.nextPage() );
  
      delay(50);
    }        
    takeLowTime = true;
  }

  //Ели движения нет
  if(digitalRead(pirPin) == LOW)
  {      
    //Если время окончания движения еще не записано
    if(takeLowTime)
    {
      lowIn = millis();          //Сохраним время окончания движения
      takeLowTime = false;       //Изменим значения флага, чтобы больше не брать время, пока не будет нового движения
    }
    //Если время без движение превышает паузу => движение окончено
    if(!lockLow && millis() - lowIn > pause)
    { 
      //Изменяем значение флага, чтобы эта часть кода исполнилась лишь раз, до нового движения
      lockLow = true;               
      
      do {
           u8g.setFont(my5x7rus);
           u8g.setScale2x2();
           u8g.drawStr(1,8,"Все ушли.");
           u8g.undoScale();
      } 
      while( u8g.nextPage() );
      
      delay(50);
    }
  }
  

}

