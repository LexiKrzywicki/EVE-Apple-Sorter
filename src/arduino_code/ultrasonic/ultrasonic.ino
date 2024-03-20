#include <Servo.h>

#define enA 11
#define in1 12
#define in2 13

const int trigPinO = 9;
const int echoPinO = 8;

long duration;
int distance;

Servo visionServo;
Servo outServo;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPinO, OUTPUT);
  pinMode(echoPinO, INPUT);
  Serial.begin(9600);
  visionServo.attach(7);
  visionServo.write(10);

  outServo.attach(2);
  outServo.write(90);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1,LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 255); 
  delay(100);

}

void loop() {



  // vision ultrasonic with servo test
  digitalWrite(trigPinO, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinO, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinO, LOW);
  duration = pulseIn(echoPinO, HIGH);
  distance = duration * 0.034 / 2;
  delay(100);
  Serial.println(distance);

  // if(distance <= 13 && distance > 5){
  //   delay(1000);
  //   visionServo.write(180);
  //   delay(2000);
  //   visionServo.write(10);
  // }

}
