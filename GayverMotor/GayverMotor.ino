/*
  Скетч к проекту "Bluetooth машинка на Arduino"
 Страница проекта (схемы, описания): https://alexgyver.ru/bluetooth-car/
 Исходники на GitHub: https://github.com/AlexGyver/BluetoothCar
 Нравится, как написан и закомментирован код? Поддержи автора! https://alexgyver.ru/support_alex/
 Автор: AlexGyver Technologies, 2018
 http://AlexGyver.ru/
 */

#define MOTOR_MAX 255	// максимальный сигнал на мотор (max 255)
#define JOY_MAX 40   	// рабочий ход джойстика (из приложения)

#include <AFMotor.h>  // Подключаем библиотеку для работы с шилдом https://github.com/adafruit/Adafruit-Motor-Shield-library/zipball/master
//#include <Servo.h>  // Подключаем библиотеку для работы с сервоприводами

// Подключаем моторы к клеммникам M1, M2, M3, M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

#define BT_TX 13
#define BT_RX 12

/*
	Bluetooth шлёт пакет вида $valueX valueY;
 	моя функция parsing разбивает этот пакет в массив intData
 	Парсинг полностью прозрачный, не блокирующий и с защитой от битого пакета,
 	так как присутствует строгий синтаксис посылки. Без хешсуммы конечно, но и так норм
 */
#define PARSE_AMOUNT 2         // число значений в массиве, который хотим получить

int intData[PARSE_AMOUNT];     // массив численных значений после парсинга
boolean recievedFlag;
int dutyR, dutyL;
int signalX, signalY;
int dataX, dataY;

// #include "GyverMotor.h"
// GMotor motorL(IN1, IN2);
// GMotor motorR(IN3, IN4);

#include <SoftwareSerial.h>
SoftwareSerial btSerial(18, 19); // TX, RX

void setup() {
  //  Serial.begin(9600);
  btSerial.begin(9600);
  //PWMfrequency(IN2, 1);   // 31 кГц
}

void loop() {
  parsing();       				// функция парсинга
  if (recievedFlag) {			// если получены данные
    recievedFlag = false;
    dataX = intData[0];
    dataY = intData[1];
    /*for (byte i = 0; i < PARSE_AMOUNT; i++) { // выводим элементы массива
     Serial.print(intData[i]); Serial.print(" ");
     } Serial.println();*/
    //    Serial.print(dutyR);
    //    Serial.print(" ");
    //    Serial.println(dutyL);
  }

  if (dataX == 0 && dataY == 0) {   // если мы в "мёртвой" зоне
    motorR_setMode(RELEASE);           // не двигаемся
    motorL_setMode(RELEASE);
    dutyR = 0;
    dutyL = dutyR;
  } 
  else {
    signalY = map((dataY), -JOY_MAX, JOY_MAX, -MOTOR_MAX, MOTOR_MAX);         // сигнал по У
    signalX = map((dataX), -JOY_MAX, JOY_MAX, -MOTOR_MAX, MOTOR_MAX); // сигнал по Х

    dutyR = signalY + signalX;
    dutyL = signalY - signalX;

    if (dutyR > 0) 
    {
      motorR_setMode(FORWARD);
    }
    else
    { 
      motorR_setMode(BACKWARD);
    }

    if (dutyL > 0) 
    {
      motorL_setMode(FORWARD);
    }
    else
    {
      motorL_setMode(BACKWARD);
    }

    dutyR = constrain(abs(dutyR), 0, MOTOR_MAX);
    dutyL = constrain(abs(dutyL), 0, MOTOR_MAX);
  }
  motorR_setSpeed(dutyR);
  motorL_setSpeed(dutyL);
}

boolean getStarted;
byte index;
String string_convert = "";
void parsing() {
  if (btSerial.available() > 0) {
    char incomingByte = btSerial.read();        // обязательно ЧИТАЕМ входящий символ
    if (getStarted) {                         // если приняли начальный символ (парсинг разрешён)
      if (incomingByte != ' ' && incomingByte != ';') {   // если это не пробел И не конец
        string_convert += incomingByte;       // складываем в строку
      } 
      else {                                // если это пробел или ; конец пакета
        intData[index] = string_convert.toInt();  // преобразуем строку в int и кладём в массив
        string_convert = "";                  // очищаем строку
        index++;                              // переходим к парсингу следующего элемента массива
      }
    }
    if (incomingByte == '$') {                // если это $
      getStarted = true;                      // поднимаем флаг, что можно парсить
      index = 0;                              // сбрасываем индекс
      string_convert = "";                    // очищаем строку
    }
    if (incomingByte == ';') {                // если таки приняли ; - конец парсинга
      getStarted = false;                     // сброс
      recievedFlag = true;                    // флаг на принятие
    }
  }
}



// движение в направлении vCmd: 
//    FORWARD  - вперед
//    BACKWARD - назад
//    RELEASE  - стоп

void motorL_setMode(uint8_t vCmd){
  motor1.run(vCmd);
  motor2.run(vCmd);
}

void motorR_setMode(uint8_t vCmd){
  motor3.run(vCmd);
  motor4.run(vCmd);
}

void motorL_setSpeed(uint8_t dutyL){
  motor1.setSpeed(dutyL);
  motor2.setSpeed(dutyL);
}

void motorR_setSpeed(uint8_t dutyR){
  motor3.setSpeed(dutyR);
  motor4.setSpeed(dutyR);
}


