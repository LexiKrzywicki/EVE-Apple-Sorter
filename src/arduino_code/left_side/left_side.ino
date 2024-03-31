//left side - L2

#include <Servo.h>


int topSwitchIV = 21;
int bttmSwitchIV = 20; 

int topSwitchRightLift = 18;
int bttmSwitchRightLift = 19;

int topSwitchLeftLift = 3;
int bttmSwitchLeftLift = 2;

//motor driver 1 - left and right lifters
int enA_1 = 8;   //right
int in1_1 = 9;   
int in2_1 = 10;
int in3_1 = 11;  //left
int in4_1 = 12;
int enB_1 = 13;

//motor driver 2 - conveyor and I->V lifter
int enA_2 = 35;  //conveyor
int in1_2 = 37;
int in2_2 = 39;
int in3_2 = 41;   //lift
int in4_2 = 43;
int enB_2 = 45;



void setup(){
  // pinMode(topSwitchIV, INPUT_PULLUP);
  // pinMode(bttmSwitchIV, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(topSwitchIV), goDownIV, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(bttmSwitchIV), goUpIV, CHANGE);
  // pinMode(enB_1, INPUT);
  // pinMode(in3_1, INPUT);
  // pinMode(in4_1, INPUT);
  // // digitalWrite(in3_1, HIGH);
  // // digitalWrite(in4_1, LOW);
  // analogWrite(enB_1, 255);

  //converyor motor
  pinMode(enA_2, OUTPUT);
  pinMode(in1_2, OUTPUT);
  pinMode(in2_2, OUTPUT);
  digitalWrite(in1_2, HIGH);
  digitalWrite(in2_2, LOW);
  analogWrite(enA_2, 255);

  //right lift
  pinMode(topSwitchRightLift, INPUT_PULLUP);
  pinMode(bttmSwitchRightLift, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(topSwitchRightLift), goDownRight, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bttmSwitchRightLift), goUpRight, CHANGE);
  pinMode(enA_1, INPUT);
  pinMode(in1_1, INPUT);
  pinMode(in2_1, INPUT);
  digitalWrite(in1_1, HIGH);
  digitalWrite(in2_1, LOW);
  analogWrite(enA_1, 255);

  //left lift
  pinMode(topSwitchLeftLift, INPUT_PULLUP);
  pinMode(bttmSwitchLeftLift, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(topSwitchLeftLift), goDownLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift), goUpLeft, CHANGE);
  pinMode(enB_1, INPUT);
  pinMode(in3_1, INPUT);
  pinMode(in4_1, INPUT);
  digitalWrite(in3_1, LOW);
  digitalWrite(in4_1, HIGH);
  analogWrite(enB_1, 255);


  delay(1000);
}

void loop(){

}


void goUpIV(){
  digitalWrite(in3_2, HIGH);
  digitalWrite(in4_2, LOW);
}

void goDownIV(){
  digitalWrite(in3_2, LOW);
  digitalWrite(in4_2, HIGH);
}

void goUpRight(){
  digitalWrite(in1_1, HIGH);
  digitalWrite(in2_1, LOW);
}

void goDownRight(){
  digitalWrite(in1_1, LOW);
  digitalWrite(in2_1, HIGH);
}

void goUpLeft(){
  digitalWrite(in3_1, HIGH);
  digitalWrite(in4_1, LOW);
}

void goDownLeft(){
  digitalWrite(in3_1, LOW);
  digitalWrite(in4_1, HIGH);
}

