int Trig = 18;
int Echo = 19; 

// the setup routine runs once when you press reset:
void setup() {                
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  Serial.begin(9600); // Инициализируем сериал порт, дабы вывести результат на монитор
  
}

unsigned int time_us=0; // Переменная для хранения временного интервала
unsigned int distance_sm=0; // Переменная для хранения расстояния в сантиметрах

void loop() {

digitalWrite(Trig, HIGH); // Подаем сигнал на выход микроконтроллера 
delayMicroseconds(10); // Удерживаем 10 микросекунд 
digitalWrite(Trig, LOW); // Затем убираем
time_us=pulseIn(Echo, HIGH); // Замеряем длину импульса 
distance_sm=time_us/58; // Пересчитываем в сантиметры
Serial.print(distance_sm); // Выводим на порт
Serial.println(""); 
delay(500); 

}
