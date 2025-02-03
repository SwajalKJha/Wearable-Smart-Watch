 #define touchPin T4
 const int ledPin = 35;
 #define LED 2

 touch_value_t THRESHOLD = 40;
 void setup() {
  // put your setup code here, to run once:
    pinMode(touchPin, GPIO_MODE_INPUT);
    pinMode(ledPin,OUTPUT);
    pinMode(LED,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  touch_value_t touchValue = touchRead(touchPin);
  if(touchValue < THRESHOLD){
    digitalWrite(ledPin,HIGH);
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(ledPin,LOW);
    digitalWrite(LED,LOW);
  }

}
