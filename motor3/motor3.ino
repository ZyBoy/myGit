#include <AFMotor.h>  // Подключаем библиотеку для работы с шилдом https://github.com/adafruit/Adafruit-Motor-Shield-library/zipball/master
#include <Servo.h>  // Подключаем библиотеку для работы с сервоприводами

// Подключаем моторы к клеммникам M1, M2, M3, M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

int i;
int val;

void setup() {
  Serial.begin(9600);
  pinMode(17, OUTPUT);
}



void loop()
{
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


void leftside(uint8_t vCmd){
  motor1.run(vCmd);
  motor2.run(vCmd);
  motor1.setSpeed(255); 
  motor2.setSpeed(255); 
}

void rightside(uint8_t vCmd){
  motor3.run(vCmd);
  motor4.run(vCmd);
  motor3.setSpeed(255); 
  motor4.setSpeed(255); 
}
