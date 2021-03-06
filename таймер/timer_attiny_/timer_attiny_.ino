/*
 * Автор: Зорин Евгений
 * Управление таймером "60 секунд" на сегментных индикаторах с общим катодом.
 * Код не оптимизирован, работает как есть.
 *
 */

#define CLOCK_PIN 0
#define RESET_PIN 1
#define BUTTON 2
#define ZUMMER 4

boolean timer_stopped = true;  // Флаг "таймер остановлен"
int vTimer = 0; // "глобальный" счетчик таймера
 
/*
 * Функция resetNumber обнуляет текущее значение на счётчике и индикаторе
 */
void resetNumber()
{
    // Даем сигнал сброса драйверу
    digitalWrite(RESET_PIN, HIGH);
    digitalWrite(RESET_PIN, LOW);
    vTimer = 0;    // и сбросим наш счетчик
}

/*
 * Функция "Щелкунчик", наращивает драйверу значение
 */
void numerator()
{
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(CLOCK_PIN, LOW);
    vTimer = vTimer + 1; // ведем счетчик для себя, т.к. с драйвера
                         // считать не получится ;)
}

/* Простая Функция генерации звука пьезоэлементом.
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

/*
 * Проверка нажатия кнопки
 */
boolean button_is_down(){
  if(digitalRead(BUTTON)==HIGH) // Проверяем нажатие кнопки, что бы можно было
                                // остановить таймер в любом состоянии
  {
    timer_stopped = true;   // переключаем флаг на "останов"
    beep(20);               // Подтверждающий бип, он же небольшая сглаживающая задержка
    resetNumber();          // обязательно сбросим таймер
    return true;            // Возвращаем подтверждение останова
  }
    return false;           // не подтверждаем нажатие кнопки
}

void setup()
{
    pinMode(RESET_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(ZUMMER, OUTPUT);   // активируем режим пина для зуммера на "выход"
    digitalWrite(BUTTON,LOW); 
    // Не забываем обнулить счётчик при старте, чтобы он не оказался в случайном состоянии
    resetNumber();
    delayMicroseconds(100);
}
 
void loop()
{
int popravka = 0; // переменная с поправкой, нужна для исправления дефекта в результате "бипа"

if(digitalRead(BUTTON)==HIGH) // Посмотрим на кнопку, актуально если таймер остановлен
{
  timer_stopped = false; // сменим флаг на противоположный, что бы запустить счетчик
  beep(20);              // подтвердим нажатие звуком
  delay(50);             // небольшая задержка, для сглаживания вероятного дребезга кнопки,
                         // вместе с бипом - более чем достаточно.
}

if (timer_stopped == false){       // Если таймер не остановлен, то запускаем цикл счетчика...
  numerator(); // повысим цифру и...
  // куча условий для писка
  if (vTimer==30){ // 30 секунд
    popravka = 20; // собственно поправка, что бы счетчик не "тормозил", она же задает звук.
    beep(popravka);
  }
  //  - если осталось последние 3 секунды таймера текущего и превышеного
  else if ((vTimer<=59 && vTimer >= 57) || (vTimer<=89 && vTimer >= 87)){
    popravka = 15; // собственно поправка, что бы счетчик не "тормозил" 
    beep(popravka);
  }
  else { // нет звука - нет поправки
    popravka = 0;
  }

 // Условие с другим звуком на лимиты таймера (60 секунд) и окончание (99 секунд).
 // Звук другой, поправка все так же нужна, счетчик дальше тикает.
   if (vTimer==60 || vTimer == 90) {
     popravka = 40; // собственно опять поправка, что бы счетчик не "тормозил" 
     beep(popravka);
   }
 
 // Каждые 5 секунд слегка попискиваем если превышен основной лимит, ибо нефиг
   if ((vTimer>60 && vTimer<90) && (vTimer%60%5 == 0)){
     popravka = 20; // и вновь поправка, что бы счетчик не "тормозил" 
     beep(popravka);
   }
 
 // А это наш "кварц", который секунды тикает нормально, с учетом поправки )))
    for (int timer_pause=0;timer_pause<(100-popravka*2);timer_pause++){
      if(button_is_down() == true) return; // Проверяем нажатие кнопки для экстренной остановки
        delayMicroseconds(1200);   // Собственно сама микропауза, которая выполненная
                                   // 1000 раз дает ~1 секунду. Подобрано опытным путем
    }

  if(vTimer == 90)
  {
    timer_stopped = true; // Если все же таймер дошел до конца, меняем флаг на "останов"
                          // принудительно, что бы без нашего желания на второй круг счетчик не пошел.
    resetNumber();
  }
}
}
