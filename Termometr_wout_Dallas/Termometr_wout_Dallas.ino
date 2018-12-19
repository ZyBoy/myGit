// Тестировалось на Arduino IDE 1.0.1
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <OneWire.h>

OneWire ds(2); // Создаем объект OneWire для шины 1-Wire, с помощью которого будет осуществляться работа с датчиком

// Подключаем экран
// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 6 - LCD chip select (CS)
// pin 7 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);

void setup() {
  Serial.begin(9600);
  display.begin();              // Инициализация дисплея
  display.setContrast(50);      // Устанавливаем контраст
  display.setTextColor(BLACK);  // Устанавливаем цвет текста
  display.setTextSize(2);       // Устанавливаем размер текста
  display.clearDisplay();       // Очищаем дисплей
  display.display();
  delay(1000); 
}

void loop() {
 // Определяем температуру от датчика DS18b20
  byte data[2]; // Место для значения температуры

  ds.reset(); // Начинаем взаимодействие со сброса всех предыдущих команд и параметров
  ds.write(0xCC); // Даем датчику DS18b20 команду пропустить поиск по адресу. В нашем случае только одно устрйоство
  ds.write(0x44); // Даем датчику DS18b20 команду измерить температуру. Само значение температуры мы еще не получаем - датчик его положит во внутреннюю память

  delay(1000); // Микросхема измеряет температуру, а мы ждем.

  ds.reset(); // Теперь готовимся получить значение измеренной температуры
  ds.write(0xCC);
  ds.write(0xBE); // Просим передать нам значение регистров со значением температуры

  // Получаем и считываем ответ
  data[0] = ds.read(); // Читаем младший байт значения температуры
  data[1] = ds.read(); // А теперь старший

  // Формируем итоговое значение:
  //    - сперва "склеиваем" значение,
  //    - затем умножаем его на коэффициент, соответсвующий разрешающей способности (для 12 бит по умолчанию - это 0,0625)
  int temperature = round(((data[1] << 8) | data[0]) * 0.0625);

  // Выводим полученное значение температуры в монитор порта
   Serial.println(temperature);

  display.clearDisplay();
  display.display();
  
  display.setTextSize(2);       // Устанавливаем размер текста
  display.setCursor(20, 16);
  display.print(temperature);
  display.setCursor(50, 16);
  display.print("C");
  display.setTextSize(1);       // Устанавливаем размер текста
  display.setCursor(44, 12);
  display.print("o");
  display.display();

 
}
