#include <Servo.h>

int topSwitchRightLift = 18;
int bttmSwitchRightLift = 19;

int topSwitchLeftLift = 3;
int bttmSwitchLeftLift = 2;

int enA_2 = 35;  //conveyor
int in1_2 = 37;
int in2_2 = 39;
int in3_2 = 41;   //lift
int in4_2 = 43;
int enB_2 = 45;

int enA_1 = 8;   //right
int in1_1 = 9;   
int in2_1 = 10;
int in3_1 = 11;  //left
int in4_1 = 12;
int enB_1 = 13;

bool upRight = false;
bool downRight = true;
bool upIV = false;
bool downIV = true;

//ultrasonics
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


void setup(){
  pinMode(trigPinV, OUTPUT);
  pinMode(echoPinV, INPUT);
  visionServo.attach(51);
  outServo.attach(53);
  pinMode(trigPinO, OUTPUT);
  pinMode(echoPinO, INPUT);
  visionServo.write(100);
  delay(25);
  outServo.write(53);
  delay(25);
  Serial.begin(9600);

  //conveyor
  pinMode(enA_2, OUTPUT);
  pinMode(in1_2, OUTPUT);
  pinMode(in2_2, OUTPUT);
  digitalWrite(in1_2, HIGH);
  digitalWrite(in2_2, LOW);
  analogWrite(enA_2, 255);

  //left lift
  pinMode(topSwitchLeftLift, INPUT_PULLUP);
  pinMode(bttmSwitchLeftLift, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(topSwitchLeftLift), goDownLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift), stop, CHANGE);
  pinMode(enB_1, INPUT);
  pinMode(in3_1, INPUT);
  pinMode(in4_1, INPUT);
  digitalWrite(in3_1, LOW);
  digitalWrite(in4_1, HIGH);
  analogWrite(enB_1, 255);
}

void loop(){
  digitalWrite(trigPinO, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinO, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinO, LOW)
  durationO = pulseIn(echoPinO, HIGH);
  distanceO = durationO * 0.034 / 2;
  delay(100);   //DELAY IS NEEDED TO NOT READ 0
  Serial.println(distanceO);
  if(distanceO <= 12){  //if distance detected, servo runs
    delay(1000);
    outServo.write(120);
    delay(1000);
    outServo.write(53);
    delay(1000);

    //raise the lift
    digitalWrite(in3_1, HIGH);
    digitalWrite(in4_1, LOW);
  }
}


void stop(){
  digitalWrite(in3_1, LOW);
  digitalWrite(in4_1, LOW);

  detachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift));
}

void goDownLeft(){
  digitalWrite(in3_1, LOW);
  digitalWrite(in4_1, HIGH);

  attachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift), stop, CHANGE);
}
