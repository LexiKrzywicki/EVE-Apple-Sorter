//left side

#include <Servo.h>

#define enA 11
#define in1 12
#define in2 13

const int trigPinV = 9;
const int echoPinV = 8;

long duration;
int distance;

Servo visionServo;
Servo outServo;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPinV, OUTPUT);
  pinMode(echoPinV, INPUT);
  Serial.begin(9600);
  visionServo.attach(7);
  visionServo.write(90);

  outServo.attach(2);
  outServo.write(10);

  // pinMode(enA, OUTPUT);
  // pinMode(in1, OUTPUT);
  // pinMode(in2, OUTPUT);
  // digitalWrite(in1,LOW);
  // digitalWrite(in2, HIGH);
  // analogWrite(enA, 255); 
  delay(100);



}

void loop() {
  // put your main code here, to run repeatedly:

}
