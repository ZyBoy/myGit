#include <MirmPS.h>
#include <MirmPS_assemf.h>

#include <AFMotor.h>  // Подключаем библиотеку для работы с шилдом https://github.com/adafruit/Adafruit-Motor-Shield-library/zipball/master
#include <Servo.h>  // Подключаем библиотеку для работы с сервоприводами

// Подключаем моторы к клеммникам M1, M2, M3, M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
// Подключаем сервопривод
Servo servo1;

// задаем разъемы для сонара и определяем переменные
int Trig = 18; // аналоговые выходы могут работать как цифровые. см. http://zelectro.com.ua/Adafruit_motor_shield
int Echo = 19; 
unsigned int time_us=0; // Переменная для хранения временного интервала
unsigned int distance_sm=0; // Переменная для хранения расстояния в сантиметрах (по умолчанию 0, что бы оглядеться)
unsigned int vDistance_sm=0;

int i;
int leftside;
int rightside;

void setup() {
  TIMSK2=1; // Обязательно для многозадачности!!!
  
  // Активируем работу с сонаром
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  // Задаем максимальную скорость вращения моторов (аналог работы PWM) 
 /* motor1.setSpeed(255);
  motor1.run(RELEASE);
  motor2.setSpeed(255);
  motor2.run(RELEASE);
  motor3.setSpeed(255);
  motor3.run(RELEASE);
  motor4.setSpeed(255);
  motor4.run(RELEASE);
  */
  servo1.attach(9);  // Подключим объект "servo1" на "девятый" пин.
  servo1.write(90);  // Начальная позиция - посередке )))
  
  pinMode(17, OUTPUT);
}

// Задаем движение машине. Учитывая, что одновременно 4 двигателя по току не стартуют, используем этот костыль.
// где:
//    vCmd - направление движения ("FORWARD" - вперед, "BACKWARD" - назад)
//    vSpeed - скорость движения ("0..25")
void gogogo(uint8_t vCmd, uint8_t vSpeed) {
  motor1.run(vCmd); // Задаем движение вперед
  motor2.run(vCmd);
  for (i=0; i<vSpeed; i++) {
    motor1.setSpeed(i*10+5); 
    motor2.setSpeed(i*10+5); 
    delay(5);
  }
  motor3.run(vCmd);
  motor4.run(vCmd);
  for (i=0; i<vSpeed; i++) {
    motor3.setSpeed(i*10+5); 
    motor4.setSpeed(i*10+5); 
    delay(5);
  }
}

// Останавливаем двигатели. Очень не рекомендуется резко переключать направление вращения двигателей.
// Лучше дать небольшой промежуток времени.
void stop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(500);  
}

void turn2left() {
    motor1.run(FORWARD);
    motor2.run(BACKWARD);
    for (i=0; i<10; i++) {
      motor1.setSpeed(i*10+5); 
      motor2.setSpeed(i*10+5); 
      delay(5);
    }
    motor3.run(BACKWARD);
    motor4.run(FORWARD);
    for (i=0; i<10; i++) {
      motor3.setSpeed(i*10+5); 
      motor4.setSpeed(i*10+5); 
      delay(5);
	}
    delay(100);
}

void turn2right() {
    motor1.run(BACKWARD);
    motor2.run(FORWARD);
    for (i=0; i<10; i++) {
      motor1.setSpeed(i*10+5); 
      motor2.setSpeed(i*10+5); 
      delay(5);
    }
    motor3.run(FORWARD);
    motor4.run(BACKWARD);
    for (i=0; i<10; i++) {
      motor3.setSpeed(i*10+5); 
      motor4.setSpeed(i*10+5); 
      delay(5);
	}
    delay(100);
}

int sonar() // функция типа int, потому что возвращается значенеи типа int )))
{
  // проверим расстояние
  digitalWrite(Trig, HIGH); // Подаем сигнал на выход микроконтроллера 
  delayMicroseconds(10); // Удерживаем 10 микросекунд 
  digitalWrite(Trig, LOW); // Затем убираем
  time_us=pulseIn(Echo, HIGH); // Замеряем длину импульса 
  vDistance_sm=time_us/58; // Пересчитываем в сантиметры
  return vDistance_sm;
}

void loop1() 
{
 if (distance_sm <= 30) // разворачиваемся, пока не появится куда ехать
  {
    stop(); // стоять, бояться!
	
	servo1.write(50);  // Посмотрим налево
        delay(500); // пауза легкая
	leftside = sonar();

	servo1.write(120);  // Посмотрим направо
        delay(500); // пауза легкая
        rightside = sonar();
	
	servo1.write(90);  // Посмотрим направо
	
	// хорошо бы еще и вперед посмотреть сразу, вдруг препятствие ушло... 
	// А так же если везде - то задний ход давать и смотреть только "право/лево" после этого.
	// Вдруг мы заехали куда-то в жопу мира, нужно оттуда выбираться же )
	
       if (leftside > rightside)
	  {
	    turn2right();
	  }
	else
	  {
	    turn2left();
	  }
  }
  distance_sm = sonar();
  // delay(500); // пауза легкая, что бы не частить.
  
  if (distance_sm > 30) // едем пока есть куда
  {
    gogogo(BACKWARD, 5); // вперед
    // строки ниже закомментил, т.к. едем не по времени, а пока не упремся во что-нибудь...
    //delay(5000); // ехать 5 секунд 
    //stop(); // стоять, бояться!
  }
}

void loop2() 
{
  // тут, в параллельном потоке, будем регулировать скорость
  
  // пока моргаем светодиодом...
  digitalWrite(17, HIGH);
  delay(1000);  
  digitalWrite(17, LOW);
  delay(1000);  
  // код для работы с источником 
}
