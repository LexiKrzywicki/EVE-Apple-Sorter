#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distanceCm;

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
  //Serial.println("START");
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  Serial.write(distanceCm);
  delay(100);
  // while(distanceCm <14){
  //   //do nothing
  // }

 //apple detected
  // while(distanceCm < 14 && distanceCm > 5){
  //   Serial.write(distanceCm);
  //   //Serial.println("Taking image");
  //   delay(1000);
  //   //Serial.println("Determine apple grade");
  //   myServo.write(30);
  //   delay(100);
  //   digitalWrite(trigPin, LOW);
  //   delayMicroseconds(2);
  //   digitalWrite(trigPin, HIGH);
  //   delayMicroseconds(10);
  //   digitalWrite(trigPin, LOW);
  //   duration = pulseIn(echoPin, HIGH);
  //   distanceCm = duration * 0.034 / 2;
  // }
  // //when no apple detected then the servo moves up
  // myServo.write(90);
  // delay(25);
  
}