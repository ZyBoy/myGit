#include <AFMotor.h>  // Подключаем библиотеку для работы с шилдом 
#include <Servo.h>  // Подключаем библиотеку для работы с сервоприводами

// Подключаем моторы к клеммникам M1, M2, M3, M4
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

int vSpeed = 255;
int i;

void setup() {
  // Задаем максимальную скорость вращения моторов (аналог работы PWM) 
  motor1.setSpeed(vSpeed);
  motor1.run(RELEASE);
  motor2.setSpeed(vSpeed);
  motor2.run(RELEASE);
  motor3.setSpeed(vSpeed);
  motor3.run(RELEASE);
  motor4.setSpeed(vSpeed);
  motor4.run(RELEASE);
}

// Задаем движение машине. Учитывая, что одновременно 4 двигателя по току не стартуют, используем этот костыль.
// где:
//    cmd - направление движения ("FORWARD" - вперед, "BACKWARD" - назад)
//    vGoSpeed - скорость движения ("0..25")
void gogogo(uint8_t cmd, uint8_t vGoSpeed) {
  motor1.run(cmd); // Задаем движение вперед
  motor2.run(cmd);
  for (i=0; i<vGoSpeed; i++) {
    motor1.setSpeed(i*10+5); 
    motor2.setSpeed(i*10+5); 
    delay(5);
 }
  motor3.run(cmd);
  motor4.run(cmd);
  for (i=0; i<vGoSpeed; i++) {
    motor3.setSpeed(i*10+5); 
    motor4.setSpeed(i*10+5); 
    delay(5);
 }
}

void stop() {
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(500);  
}

void loop() 
{
  gogogo(FORWARD, 25);
  delay(5000);
  stop();
  
  gogogo(BACKWARD, 25);
  delay(5000);
  stop();

  
  /*
  motor1.run(FORWARD); // Задаем движение вперед
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  for (i=0; i<255; i++) {
    motor1.setSpeed(vSpeed); 
    motor2.setSpeed(vSpeed); 
    delay(5);
 }
  for (i=0; i<255; i++) {
    motor3.setSpeed(vSpeed); 
    motor4.setSpeed(vSpeed); 
    delay(5);
 }

  delay(1000);
  
  // Останавливаем двигатели
  // Очень не рекомендуем резко переключать направление вращения двигателей.
  // Лучше дать небольшой промежуток времени.
  
  motor1.run(RELEASE); 
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(500);
  
  // Двигаемся в обратном направлении
  motor1.run(BACKWARD);  // Задаем движение назад
  motor2.run(BACKWARD);
  for (i=0; i<255; i++) {
    motor1.setSpeed(vSpeed); 
    motor2.setSpeed(vSpeed); 
    delay(5);
 }
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  for (i=0; i<255; i++) {
    motor3.setSpeed(vSpeed); 
    motor4.setSpeed(vSpeed); 
    delay(5);
 }
  delay(1000);
  
  // Останавливаем двигатели  
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(500);*/
  
  // разворачиваемся
    // Двигаемся в обратном направлении
  motor1.run(FORWARD);  // Задаем движение назад
  motor2.run(BACKWARD);
  for (i=0; i<25; i++) {
    motor1.setSpeed(i*10+5); 
    motor2.setSpeed(i*10+5); 
    delay(5);
 }
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
  for (i=0; i<25; i++) {
    motor3.setSpeed(i*10+5); 
    motor4.setSpeed(i*10+5); 
    delay(5);
 }
  delay(5000);
  
  // Останавливаем двигатели  
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(1000);
}
