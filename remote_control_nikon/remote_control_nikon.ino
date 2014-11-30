
int outPin = 12;

void setup() {
  // put your setup code here, to run once:
  pinMode(outPin, OUTPUT);
  digitalWrite(outPin, LOW);
}

void shot(void){
  digitalWrite(outPin, HIGH);   // sets the pin on
  delayMicroseconds(2000);        // pauses
  digitalWrite(outPin, LOW);    // sets the pin off
  delay(28);        // pauses

  digitalWrite(outPin, HIGH);   // sets the pin on
  delayMicroseconds(400);        // pauses
  digitalWrite(outPin, LOW);    // sets the pin off
  delayMicroseconds(1580);        // pauses

  digitalWrite(outPin, HIGH);   // sets the pin on
  delayMicroseconds(400);        // pauses
  digitalWrite(outPin, LOW);    // sets the pin off
  delayMicroseconds(3580);        // pauses

  digitalWrite(outPin, HIGH);   // sets the pin on
  delayMicroseconds(400);        // pauses
  
  digitalWrite(outPin, LOW);
}


void loop() {
  // put your main code here, to run repeatedly:
  shot();
  delay(2000);        // pauses
}
