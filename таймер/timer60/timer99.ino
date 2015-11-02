/* СООТВЕТСТВИЕ ПИНОВ (по факту)
2  - КНОПКА
3  -  8 LED "c2"
4  -  7 LED "g2"
5  -  6 LED "d2"
6  -  5 LED "e2"
7  -  3 LED "c1"
8  -  2 LED "d1"
9  -  1 LED "e1"
10 - ЗУММЕР
11 - 18 LED "f1"
12 - 17 LED "g1"
13 - 16 LED "a1"
A0 (14) - 15 LED "b1"
A1 (15) - 12 LED "f2"
A2 (16) - 11 LED "a2"
A3 (17) - 10 LED "b2"
*/

// Определим синонимы и глобальные переменные
#define BUTTON 2  // Дадим название пину кнопки
#define ZUMMER 10 // Дадим название пину пищалки

byte    d_map[10];        // память знакогенератора (массив)
byte    l_pin[14];        // мап пинов на сегменты знакоместа (массив)

boolean timer_stopped = true;  // Флаг "таймер остановлен"


// Предустановки и инициализации
void setup() {
  pinMode(ZUMMER, OUTPUT);   // активируем режим пина для зуммера на "выход"
  digitalWrite(ZUMMER,LOW);  // запишем туда "нулевой уровень" (зачем там постояное питание? пищать все равно не будет, но все же)
  digitalWrite(BUTTON,HIGH); // запишем в кнопку "высокий уровень" (активируем "внутренние" резисторы, иначе не работает одновреенно с сегментами, без них норм, что странно)

// массив соответствия секторов индикатора пинам (по факту)  
  l_pin[0]  = 13;  // A 1
  l_pin[1]  = 14;  // B 1
  l_pin[2]  = 7;   // C 1
  l_pin[3]  = 8;   // D 1
  l_pin[4]  = 9;   // E 1
  l_pin[5]  = 11;  // F 1
  l_pin[6]  = 12;  // G 1
  l_pin[7]  = 16;  // A 2
  l_pin[8]  = 17;  // B 2
  l_pin[9]  = 3;   // C 2
  l_pin[10] = 5;   // D 2
  l_pin[11] = 6;   // E 2
  l_pin[12] = 15;  // F 2
  l_pin[13] = 4;   // G 2

// Инициализация пинов (в цикле) для работы с сегментами индикатора
  for (byte i=0;i<14;i++) {
    pinMode(l_pin[i],OUTPUT);    // активируем "выход"
    digitalWrite(l_pin[i],HIGH); // активируем "внутренние" резисторы
 }

// массив "карта символов". Теоретически можно расширить, но нам достаточно
  d_map[0]  = 0b00000010; // 0
  d_map[1]  = 0b10011110; // 1
  d_map[2]  = 0b00100100; // 2
  d_map[3]  = 0b00001100; // 3 
  d_map[4]  = 0b10011000; // 4
  d_map[5]  = 0b01001000; // 5
  d_map[6]  = 0b01000000; // 6
  d_map[7]  = 0b00011110; // 7
  d_map[8]  = 0b00000000; // 8
  d_map[9]  = 0b00001000; // 9

}

/* Функция отображения символа на индикаторе
 * Входные параметры
 *   byte d        - символ, который нужно отобразить (соответствует массиву "карта символов" d_map[])
 *   boolean left  - признак сегмента (true - первый, false - левый), т.к. у нас индикатор двухсегментный.
 */
void set_digit(byte d, boolean left) {
  byte i=0;          // служебная переменная, счетчик, который обходит пины по порядку.
  byte sdvig=0;      // коррекция счетчика пинов (в массиве l_pin[] левые с 0, правые с 7)
  if (left == false) // Если не левый индикатор, то увеличиваем счетчик
    sdvig=7;
  
  for (byte mask=0x80;mask!=1;mask>>=1) // цикл обхода маски (сдвигом на 1) для нужного символа
    digitalWrite(l_pin[sdvig+i++],(d_map[d]&mask) ? HIGH : LOW ); // записываем в соответствующий пин значение взятое из маски
}


/* Функция генерации звука пьезоэлементом. Все достаточно тупо и просто ))))
 * Входные параметры
 *   int vDelay    - период в микросекундах, по которому генерится звук.
 */
void beep(int vDelay){
  for (int i=0;i<=1000;i++){ // собственно "колебаем" пьезо 1000 раз
    digitalWrite(ZUMMER, HIGH);   // подаем на пьезо напряжение
    delayMicroseconds(vDelay);    // ждем "период"
    digitalWrite(ZUMMER, LOW);    // снимаем напряжение
    delayMicroseconds(vDelay);    // и опять ждем
  }
}

/* Функция проверки нажатия кнопки.
 * Входных параметров нет, не нужны же )
 */
boolean button_is_down(){
  if(digitalRead(BUTTON)==LOW) // Проверяем нажатие кнопки, что бы можно было остановить таймер в любом состоянии
  {
    timer_stopped = true;   // переключаем флаг на "останов"
    beep(200);              // Подтверждающий бип, он же небольшая сглаживающая задержка
    return true;            // Возвращаем подтверждение останова
  }
    return false;           // не подтверждаем нажатие кнопки
}

// Основная функция которая выполняется непрерывно
void loop() {
int popravka = 0; // переменная с поправкой, нужна для исправления дефекта в результате "бипа"
int ten = 0;               //десятки
int one = 0;               //единицы
    
//Включим экран
set_digit(0, true);   // отображаем ноль слева
set_digit(0, false);  // ... и справа. 

// Проверка на нажатие кнопки
if(digitalRead(BUTTON)==LOW) // т.к. включен внутренний подтягивающий резистор, то LOW
{
  // Нажатие сработало
  timer_stopped = false; // сменим флаг на противоположный, что бы запустить счетчик
  beep(200);             // подтвердим нажатие звуком
  delay(500);            // небольшая задержка, для сглаживания вероятного дребезга кнопки, вместе с бипом - более чем достаточно.
}

// Можно было вынести в отдельную функцию, но смысла нет.
if (timer_stopped == false){       // Если таймер не остановлен, то запускаем циклы счетчика...

  for (int dtimer=0;dtimer<100;dtimer++){
    ten=dtimer/10;               //десятки
    one=dtimer%10;               //единицы
      set_digit(ten, true);      // рисуем цифру слева
      set_digit(one, false);     // ... и справа
      
      // Дополнительное условие для звукового сигнала
      //  - если отображается 30 секунд
        if (dtimer==30){
          beep(100);
          popravka = 100; // собственно поправка, что бы счетчик не "тормозил" 
        }
      //  - если отображаются последние 3 секунды таймера текущего и превышеного
        else if ((dtimer<=59 && dtimer >= 57) || (dtimer<=98 && dtimer >= 96)){
          beep(200);
          popravka = 200; // собственно поправка, что бы счетчик не "тормозил" 
        }
        else { // нет звука - нет поправки
          popravka = 0;
        }
      
      // Условие на превышение таймера (60 секунд) и окончание (99 секунд). Звук другой, поправка все так же нужна, счетчик дальше тикает.
        if (dtimer==60 || dtimer==99) {
          beep(400);
          popravka = 400; // собственно опять поправка, что бы счетчик не "тормозил" 
        }
      
      // Дальше каждые 5 секунд слегка попискиваем, ибо нефиг
        if (dtimer>60 && (one == 0 || one == 5){
          beep(100);
          popravka = 100; // собственно поправка, что бы счетчик не "тормозил" 
        }
      
      // Цикл с паузой, что бы секунды тикали нормально, с учетом поправки )))
      for (int timer_pause=0;timer_pause<(1000-popravka);timer_pause++){
        if(button_is_down() == true) return;   // Проверим нажатость кнопки и при необходимости прерываем текущее выполнение функции loop, и все начинается с начала )))
        delayMicroseconds(1000);   // Собственно сама микропауза, которая выполненная 1000 раз дает 1 секунду.
      }
  }

  timer_stopped = true; // Если все же таймер дошел до конца, меняем флаг на "останов" принудительно, что бы без нашего желания на второй круг счетчик не пошел.
  delay(500);           // пауза уже не особо нужна тут, просто сохранилась из предыдущих редакций.
}
}

// все, конец файла.
