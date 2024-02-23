#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distanceCm = 30;  //set to 30 to get into while loop

Servo myServo;
int pos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(11);
  myServo.write(90);
  delay(25);
}
void loop() {
  //Serial.println("START");{

    if(Serial.availableForWrite()){
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distanceCm = duration * 0.034 / 2;
      Serial.write(distanceCm);
    }
    else{
      //Serial.print("made it here");
      // Serial.end();
      // delay(100);
      // Serial.begin(9600);
      // delay(100);
      if(Serial.available() >0 ){
        delay(2000);
        myServo.write(30);
        delay(2000);
        myServo.write(90);
      }
    }
    delay(100);
  
}