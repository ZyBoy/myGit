/*
  Электронный распознаватель монет (по размеру) для копилки со счётчиком суммы и статистикой по каждому типу монет.
  Функционал:
  Распознавание размера монет с высокой точностью и его привязка к стоимости каждой монеты
  Вычисление общей суммы монет в копилке
  Статистика по числу монет каждого типа
  Все настройки сохраняются в энергонезависимую память и не сбрасываются при питании
  Накопленная сумма тоже хранится в энергонезависимой памяти и не боится сбоев питания
  Режим глубокого энергосбережения: в спящем режиме потребляется 0.07 мА, в схеме без преобразователя 0.02 мА
  Поддержка любого числа монет разного размера
  Автоматическая калибровка типов монет
  Сброс накопленного количества
  Подробности в видео: https://youtu.be/lH4qfGlK2Qk
  Created 2017
  by AlexGyver
  AlexGyver Home Labs Inc.
*/

//-------НАСТРОЙКИ---------
//#define coin_amount 4    // число монет, которые нужно распознать
//float coin_value[coin_amount] = {1.0, 2.0, 5.0, 10.0};  // стоимость монет
#define coin_amount 1    // число монет, которые нужно распознать
float coin_value[coin_amount] = {10.0};  // стоимость монет
String currency = "RUB"; // валюта (английские буквы!!!)
String wtf = "На 3D-принтер"; // на что копим?
int stb_time = 10000;    // время бездействия, через которое система уйдёт в сон (миллисекунды)
//-------НАСТРОЙКИ---------

int coin_signal[coin_amount];    // тут хранится значение сигнала для каждого размера монет
int coin_quantity[coin_amount];  // количество монет
byte empty_signal;               // храним уровень пустого сигнала
unsigned long standby_timer, reset_timer; // таймеры
float summ_money = 0;            // сумма монет в копилке

//-------БИБЛИОТЕКИ---------
#include "LowPower.h"
#include "EEPROMex.h"
#include <iarduino_OLED_txt.h>                         // Подключаем библиотеку iarduino_OLED_txt.
//-------БИБЛИОТЕКИ---------
iarduino_OLED_txt myOLED(0x78);                        // Объявляем объект myOLED, указывая адрес дисплея на шине I2C: 0x78 (если учитывать бит RW=0).
extern uint8_t SmallFontRus[];                         // Подключаем шрифт SmallFontRus.

boolean recogn_flag, sleep_flag = true;   // флажки
//-------КНОПКИ---------
#define button 2         // кнопка "проснуться"
#define calibr_button 3  // скрытая кнопка калибровкии сброса
#define disp_power 12    // питание дисплея
#define LEDpin 11        // питание светодиода
#define IRpin 17         // питание фототранзистора
#define IRsens 14        // сигнал фототранзистора
//-------КНОПКИ---------
int sens_signal, last_sens_signal;
boolean coin_flag = false;

void setup() {
  Serial.begin(9600);                   // открыть порт для связи с ПК для отладки
  delay(500);

  // подтягиваем кнопки
  pinMode(button, INPUT_PULLUP);
  pinMode(calibr_button, INPUT_PULLUP);

  // пины питания как выходы
  pinMode(disp_power, OUTPUT);
  pinMode(LEDpin, OUTPUT);
  pinMode(IRpin, OUTPUT);

  // подать питание на дисплей и датчик
  digitalWrite(disp_power, 1);
  digitalWrite(LEDpin, 1);
  digitalWrite(IRpin, 1);

  // подключить прерывание
  attachInterrupt(0, wake_up, CHANGE);

  empty_signal = analogRead(IRsens);  // считать пустой (опорный) сигнал
  myOLED.begin();                                    // Инициируем работу с дисплеем.
  myOLED.setFont(SmallFontRus);                      // Указываем шрифт который требуется использовать для вывода цифр и текста.

  if (!digitalRead(calibr_button)) {  // если при запуске нажата кнопка калибровки
    myOLED.clrScr();                                   // Чистим экран.
    myOLED.print("Сервис", 0, 0);
    delay(500);
    reset_timer = millis();
    while (1) {                                   // бесконечный цикл
      if (millis() - reset_timer > 3000) {        // если кнопка всё ещё удерживается и прошло 3 секунды
        // очистить количество монет
        for (byte i = 0; i < coin_amount; i++) {
          coin_quantity[i] = 0;
          EEPROM.writeInt(20 + i * 2, 0);
        }
        myOLED.clrScr();
        myOLED.print("Память очищена", 0, 0);
        delay(100);
      }
      if (digitalRead(calibr_button)) {   // если отпустили кнопку, перейти к калибровке
        myOLED.clrScr();
        myOLED.print("Калибровка", 0, 0);
        break;
      }
    }
    while (1) {
      for (byte i = 0; i < coin_amount; i++) {
        myOLED.print(coin_value[i], 0, 1); // отобразить цену монеты, размер которой калибруется
        myOLED.print(currency, 52, 1);    // отобразить валюту
        last_sens_signal = empty_signal;
        while (1) {
          sens_signal = analogRead(IRsens);                                    // считать датчик
          if (sens_signal > last_sens_signal) last_sens_signal = sens_signal;  // если текущее значение больше предыдущего
          if (sens_signal - empty_signal > 3) coin_flag = true;                // если значение упало почти до "пустого", считать что монета улетела
          if (coin_flag && (abs(sens_signal - empty_signal)) < 2) {            // если монета точно улетела
            coin_signal[i] = last_sens_signal;                                 // записать максимальное значение в память
            EEPROM.writeInt(i * 2, coin_signal[i]);
            coin_flag = false;
            break;
          }
        }
      }
      break;
    }
  }

  // при старте системы считать из памяти сигналы монет для дальнейшей работы, а также их количество в банке
  for (byte i = 0; i < coin_amount; i++) {
    coin_signal[i] = EEPROM.readInt(i * 2);
    coin_quantity[i] = EEPROM.readInt(20 + i * 2);
    summ_money += coin_quantity[i] * coin_value[i];  // ну и сумму сразу посчитать, как произведение цены монеты на количество
  }

 /*
    // для отладки, вывести сигналы монет в порт
    for (byte i = 0; i < coin_amount; i++) {
      Serial.println(coin_signal[i]);
    }
 */
  standby_timer = millis();  // обнулить таймер ухода в сон
}

void loop() {
  if (sleep_flag) {  // если проснулись  после сна, инициализировать дисплей и вывести текст, сумму и валюту
    delay(500);
    myOLED.begin();
    myOLED.clrScr();
    myOLED.print(wtf, OLED_C, 1);
    myOLED.print(summ_money, OLED_C, 4);
    myOLED.print(currency, OLED_C, 6);
    empty_signal = analogRead(IRsens);
    sleep_flag = false;
  }

  // далее работаем в бесконечном цикле
  last_sens_signal = empty_signal;
  while (1) {
    sens_signal = analogRead(IRsens);  // далее такой же алгоритм, как при калибровке
    if (sens_signal > last_sens_signal) last_sens_signal = sens_signal;
    if (sens_signal - empty_signal > 3) coin_flag = true;
    if (coin_flag && (abs(sens_signal - empty_signal)) < 2) {
      recogn_flag = false;  // флажок ошибки, пока что не используется
      // в общем нашли максимум для пролетевшей монетки, записали в last_sens_signal
      // далее начинаем сравнивать со значениями для монет, хранящимися в памяти
      for (byte i = 0; i < coin_amount; i++) {
        int delta = abs(last_sens_signal - coin_signal[i]);   // вот самое главное! ищем АБСОЛЮТНОЕ (то бишь по модулю)
        // значение разности полученного сигнала с нашими значениями из памяти
        if (delta < 30) {   // и вот тут если эта разность попадает в диапазон, то считаем монетку распознанной
          summ_money += coin_value[i];  // к сумме тупо прибавляем цену монетки (дада, сумма считается двумя разными способами. При старте системы суммой всех монет, а тут прибавление
          myOLED.print(summ_money, OLED_C, 4);
   //       Serial.println(coin_value[i]);
 //         Serial.println(summ_money);
          coin_quantity[i]++;  // для распознанного номера монетки прибавляем количество
          recogn_flag = true;
          break;
        }
      }
      coin_flag = false;
      standby_timer = millis();  // сбросить таймер
      break;
    }

    // если ничего не делали, времят аймера вышло, спим
    if (millis() - standby_timer > stb_time) {
      good_night();
      break;
    }

    // если монетка вставлена (замыкает контакты) и удерживается 2 секунды
    while (!digitalRead(button)) {
      if (millis() - standby_timer > 2000) {
        myOLED.clrScr();

        // отобразить на дисплее: сверху цены монет (округлено до целых!!!!), снизу их количество
        for (byte i = 0; i < coin_amount; i++) {
          myOLED.print((int)coin_value[i], i * 3 * 4, 0);
          myOLED.print(coin_quantity[i], i * 3 * 4, 1);
        }
      }
    }
  }
}

// функция сна
void good_night() {
  // перед тем как пойти спать, записываем в EEPROM новые полученные количества монет по адресам начиная с 20го (пук кек)
  for (byte i = 0; i < coin_amount; i++) {
    EEPROM.updateInt(20 + i * 2, coin_quantity[i]);
  }
  sleep_flag = true;
  // вырубить питание со всех дисплеев и датчиков
  digitalWrite(disp_power, 0);
  digitalWrite(LEDpin, 0);
  digitalWrite(IRpin, 0);
  delay(100);
  // и вот теперь спать
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

// просыпаемся по ПРЕРЫВАНИЮ (эта функция - обработчик прерывания)
void wake_up() {
  // возвращаем питание на дисплей и датчик
  digitalWrite(disp_power, 1);
  digitalWrite(LEDpin, 1);
  digitalWrite(IRpin, 1);
  standby_timer = millis();  // и обнуляем таймер
}
// Спасибо за внимание, ваш Алекс
