#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
 
// Номер (8) пина Arduino с подключенным датчиком
#define PIN_DS18B20 8

// Подключаем экран
// pin 3 - Serial clock out (SCLK)
// pin 4 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 6 - LCD chip select (CS)
// pin 7 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7);

// Создаем объект OneWire
OneWire oneWire(PIN_DS18B20);
 
// Создаем объект DallasTemperature для работы с сенсорами, передавая ему ссылку на объект для работы с 1-Wire.
DallasTemperature dallasSensors(&oneWire);
 
// Специальный объект для хранения адреса устройства
DeviceAddress sensorAddress;
 
void setup(void){
  Serial.begin(9600);
  Serial.println("Пример работы с датчиком температуры ds18b20 с помощью библиотеки DallasTemperature");

  display.begin();              // Инициализация дисплея
  display.setContrast(60);      // Устанавливаем контраст
  display.setTextColor(BLACK);  // Устанавливаем цвет текста
  display.setTextSize(1);       // Устанавливаем размер текста
  display.clearDisplay();       // Очищаем дисплей
  display.display();
//  delay(1000); 
 
  // Выполняем поиск устрйоств на линии
  dallasSensors.begin();
 
  // Определяем режим питания (по отдельной линии или через паразитное питание по линии данных)
  Serial.print("Режим паразитного питания: ");
  if (dallasSensors.isParasitePowerMode()) Serial.println("ВКЛЮЧЕН");
  else Serial.println("ВЫКЛЮЧЕН");
 
  // Раскомментируйте, если хотите задать адрес устройства вручную
  //sensorAddress = { 0x28, 0x1D, 0x39, 0x31, 0x2, 0x0, 0x0, 0xF0 };
 
  // Поиск устройства:
  // Ищем адрес устройства по порядку (индекс задается вторым параметром функции)
  if (!dallasSensors.getAddress(sensorAddress, 0)) Serial.println("Не можем найти первое устройство");
 
  // Отображаем адрес ds18b20, который мы нашли
  Serial.print("Адрес устройства: ");
  printAddress(sensorAddress);
  Serial.println();
 
  // Устанавливаем разрешение датчика в 12 бит (мы могли бы установить другие значения, точность уменьшится, но скорость получения данных увеличится
  dallasSensors.setResolution(sensorAddress, 12);
}
 
void loop(void){
  // Запрос на измерения датчиком температуры
  dallasSensors.requestTemperatures(); // Просим ds18b20 собрать данные
   //  Запрос на получение сохраненного значения температуры
  printTemperature(sensorAddress);
   // Задержка для того, чтобы можно было что-то разобрать на экране
  delay(1000);
}
 
// Вспомогательная функция печати значения температуры для устрйоства
void printTemperature(DeviceAddress deviceAddress){
  float tempC = dallasSensors.getTempC(deviceAddress);
  Serial.println(tempC);
  Serial.println("Привет");

  display.clearDisplay();
  display.display();

  // Выведем текст
  display.print("Temp ");
  display.print(tempC);
  
  display.display();
}
 
// Вспомогательная функция для отображения адреса датчика ds18b20
void printAddress(DeviceAddress deviceAddress){
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
