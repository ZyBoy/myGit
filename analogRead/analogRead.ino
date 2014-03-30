// These constants won't change:
const int analogPin = A0;    // pin that the sensor is attached to // срабатывает в общем-то от обычного проводка, прицепленого к пину, к которому подносишь руку (внезапно, да?)
const int ledPin = 17;       // pin that the LED is attached to
const int threshold = 1000;   // an arbitrary threshold level that's in the range of the analog input // порог срабатывания, максимум 1023 кажется

int chek = 1; // флаг состояния лампочки ("=1" и ">1")

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // read the value of the potentiometer:
  int analogValue = analogRead(analogPin); // снимем показания с аналогового входа

  // if the analog value is high enough, turn on the LED:
  if (analogValue > threshold) { // если преодолели порог срабатывания, то ...
      if (chek > 1) // если лампочка выключена, то ...
      {
        chek = 1; // сменим флаг состояния
        digitalWrite(ledPin, HIGH); // включим лампочку
      }
      else // иначе лампочка включена
      {
        chek = 2; // сменим флаг состояния
        digitalWrite(ledPin,LOW);  // выключим лампочку
      }
    delay(500); // поставим задержку. лампочка все равно будет светить, но это избавит нас от "дребезга". Т.е. многократного срабатывания в короткий промежуток времени.
  } 
}

