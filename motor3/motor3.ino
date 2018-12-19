#include <AFMotor.h>  // Подключаем библиотеку для работы с шилдом https://github.com/adafruit/Adafruit-Motor-Shield-library/zipball/master
#include <Servo.h>  // Подключаем библиотеку для работы с сервоприводами
#include <PS2X_lib.h>  //для v1.6

// Подключаем моторы к клеммникам M1, M2, M3, M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

/******************************************************************
   укажите пины к которым подключен PS2 джойстик:
     - 1-я колонка: оригинал
     - 2-я колонка: Stef?
   замените номера пинов на те которые вы используете
 ******************************************************************/
#define PS2_DAT        14    
#define PS2_CMD        15
#define PS2_SEL        16
#define PS2_CLK        17

/******************************************************************
   выберете режимы PS2 контроллера:
     - pressures = аналоговое считывание нажатия кнопок
     - rumble    = вибромоторы
   откройте или закройте комментарием каждую функцию
 ******************************************************************/
//#define pressures   true
#define pressures   false
//#define rumble      true
#define rumble      false

PS2X ps2x; // создание класса для PS2 контроллера

//на данный момент библиотека НЕ поддерживает подключение контроллера на горячую, это означает
//вы всегда должны либо перезапустить Ардуино
//либо вызвать config_gamepad(pins) после подключения джойстика.

int error = 0;
byte type = 0;
byte vibrate = 0;



int i;
int val;

void setup() {
  Serial.begin(57600);
  pinMode(17, OUTPUT);

  //установка пинов и настроек: GamePad(clock, command, attention, data, Pressures?, Rumble?) проверка ошибок
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

  if (error == 0) {

    // Сонтроллера найден, всё настроено удачно
    Serial.print("Found Controller, configured successful ");

    Serial.print("pressures = ");
    if (pressures)
      Serial.println("true ");
    else
      Serial.println("false");
    Serial.print("rumble = ");
    if (rumble)
      Serial.println("true)");
    else
      Serial.println("false");

    // Попробуйте все кнопки, при нажатии X контроллера будет вибрировать, чем дольше нажатие, тем быстрее вибрация
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");

    // Зажмите L1 или R1, будут отображаться показания аналоговых стикеров
    Serial.println("holding L1 or R1 will print out the analog stick values.");

    // Дополнительно: Зайдите на www.billporter.info узнайте об обновлениях или сообщите об ошибке
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }
  else if (error == 1)
    // Контроллер не найден, проверьте провода, посмотрите readme.txt для включения функции выявления ошибок. Посетите www.billporter.info и узнайте об известных проблемах
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

  else if (error == 2)
    // Контроллер найден, но не реагирует на команды. Посмотрите readme.txt для включения функции выявления ошибок. Посетите www.billporter.info и узнайте об известных проблемах
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if (error == 3)
    // Контроллер не принимает режим Pressures, возможно он им не поддерживается.
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

  //  Serial.print(ps2x.Analog(1), HEX);

  type = ps2x.readType();
  switch (type) {
    case 0:
      // Найден неизвестный тип контроллера
      Serial.print("Unknown Controller type found ");
      break;
    case 1:
      //Найден DualShock контроллер
      Serial.print("DualShock Controller found ");
      break;
    case 2:
      //Найден DualShock контроллер
      Serial.print("GuitarHero Controller found ");
      break;
    case 3:
      // Найден Беспроводной Sony DualShock контроллер
      Serial.print("Wireless Sony DualShock Controller found ");
      break;
  }
}



void loop()
{

  if (error == 1) //Цикл «loop» пропускается если джойстик не найден
    return;

  ps2x.read_gamepad(false, vibrate); //считывание данных с джойстика и установка скорости вибрации

  if (ps2x.Button(PSB_PAD_UP)) {     //будет TRUE пока кнопка нажата
    forward();
  }
  else if (ps2x.Button(PSB_PAD_RIGHT)) {
    right();
  }
  else if (ps2x.Button(PSB_PAD_LEFT)) {
    left();
  }
  else if (ps2x.Button(PSB_PAD_DOWN)) {
    backward();
  }
  else
  {
    stop();
  }
  /*
    if (Serial.available())
    {
      val = Serial.read();

      // Задаём движение вперёд
      if (val == 'D')
      {
        forward();
      }

      // Задаём движение назад
      if ( val == 'A')
      {
        backward();
      }

      // Задаём движение вправо
      if ( val == 'S')
      {
        right();
      }

      // Задаём движение влево
      if ( val == 'W')
      {
        left();
      }

      // Стоп режим
      // При отпускании клавиш в программе в порт шлется «T»
      if ( val == 'T') // При нажатии клавиши «T»
      {
        stop();
      }
    }
  */
}


// движение в направлении vCmd:
//    FORWARD  - вперед
//    BACKWARD - назад
//    RELEASE  - стоп
void run(uint8_t vCmd) {
  rightside(vCmd);
  leftside(vCmd);
}

void stop() {
  run(RELEASE);
}

void forward() {
  run(FORWARD);
}

void backward() {
  run(BACKWARD);
}

void left() {
  rightside(FORWARD);
  leftside(BACKWARD);
}

void right() {
  rightside(BACKWARD);
  leftside(FORWARD);
}


void leftside(uint8_t vCmd) {
  motor1.run(vCmd);
  motor2.run(vCmd);
  motor1.setSpeed(255);
  motor2.setSpeed(255);
}

void rightside(uint8_t vCmd) {
  motor3.run(vCmd);
  motor4.run(vCmd);
  motor3.setSpeed(255);
  motor4.setSpeed(255);
}
