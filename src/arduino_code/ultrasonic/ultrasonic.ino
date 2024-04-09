#include <Servo.h>

#define enA 11
#define in1 12
#define in2 13

const int trigPinV = 47;
const int echoPinV = 49;
const int trigPinO = 7;
const int echoPinO = 6;
long durationV;
int distanceV = 30;
long durationO;
int distanceO;

Servo visionServo;
Servo outServo;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPinV, OUTPUT);
  pinMode(echoPinV, INPUT);
  visionServo.attach(51);
  outServo.attach(53);
  pinMode(trigPinO, OUTPUT);
  pinMode(echoPinO, INPUT);
  visionServo.write(100);
  delay(25);
  outServo.write(55);
  delay(25);
  Serial.begin(9600);
  delay(100);

}

void loop() {



  // vision ultrasonic with servo test
  digitalWrite(trigPinV, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinV, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinV, LOW);
  durationV = pulseIn(echoPinV, HIGH);
  distanceV = durationV * 0.034 / 2;
  delay(100);
  Serial.println(distanceV);

  // if(distance <= 13 && distance > 5){
  //   delay(1000);
  //   visionServo.write(180);
  //   delay(2000);
  //   visionServo.write(10);
  // }

}
