// constants won't change. They're used here to 
// set pin numbers:
const int LeftPin = 3;     // the number of the leftLED pin
//#define LeftPin 3
const int RightPin = 4;     // the number of the rightLED pin
const int LeftledPin =  1;      // the number of the LeftLED
const int RightledPin =  0;      // the number of the RightLED

void setup() {
  // initialize the LED pin as an output:
  pinMode(RightledPin, OUTPUT);
  pinMode(LeftledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(LeftPin, INPUT);     
  pinMode(RightPin, INPUT);     
}

void loop(){
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (digitalRead(RightPin) == HIGH) {     
    // turn LED on:    
    digitalWrite(RightledPin, HIGH);  
    delay(50);
  }
  else if (digitalRead(LeftPin) == HIGH) {     
    // turn LED on:    
    digitalWrite(LeftledPin, HIGH);  
    delay(50);
  }  
  digitalWrite(RightledPin, LOW); 
  digitalWrite(LeftledPin, LOW); 
  delay(50);

  /* 
  digitalWrite(RightledPin, HIGH); 
   delay(50);
  digitalWrite(RightledPin, LOW); 
    delay(50); 
  */
}
