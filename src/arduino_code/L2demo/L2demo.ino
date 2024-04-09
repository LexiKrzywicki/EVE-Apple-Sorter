//left side - L2

#include <Servo.h>


int topSwitchIV = 21;
int bttmSwitchIV = 20; 

int topSwitchRightLift = 3;
int bttmSwitchRightLift = 2;

int topSwitchLeftLift = 19;
int bttmSwitchLeftLift = 19;

//ultrasonics
const int trigPinV = 47;
const int echoPinV = 49;
const int trigPinO = 7;
const int echoPinO = 6;
long durationV;
int distanceV = 30;
long durationO;
int distanceO;

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

bool upRight = false;
bool downRight = true;
bool downLeft = true;
bool upLeft = false;
bool upIV = false;
bool downIV = true;

Servo visionServo;
Servo outServo;

char stage = 'I';
int count = 0;

void setup(){
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

  //I-V lift
  pinMode(topSwitchIV, INPUT_PULLUP);
  pinMode(bttmSwitchIV, INPUT_PULLUP);
  pinMode(enB_2, INPUT);
  pinMode(in3_2, INPUT);
  pinMode(in4_2, INPUT);
  digitalWrite(in3_2, LOW);
  digitalWrite(in4_2, HIGH);
  upIV = false;
  downIV = true;
  analogWrite(enB_2, 255);

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
  pinMode(enA_1, INPUT);
  pinMode(in1_1, INPUT);
  pinMode(in2_1, INPUT);
  digitalWrite(in1_1, HIGH);
  digitalWrite(in2_1, LOW);
  upRight = true;
  downRight = false;
  analogWrite(enA_1, 255);

  // //left lift
  pinMode(topSwitchLeftLift, INPUT_PULLUP);
  pinMode(bttmSwitchLeftLift, INPUT_PULLUP);
  pinMode(enB_1, INPUT);
  pinMode(in3_1, INPUT);
  pinMode(in4_1, INPUT);
  digitalWrite(in3_1, LOW);
  digitalWrite(in4_1, HIGH);
  upRight = false;
  downRight = true;
  analogWrite(enB_1, 255);


  delay(1000);

  attachInterrupt(digitalPinToInterrupt(topSwitchRightLift), goDownRight, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bttmSwitchRightLift), goUpRight, CHANGE);
  attachInterrupt(digitalPinToInterrupt(topSwitchLeftLift), goDownLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift), stop, CHANGE);
  attachInterrupt(digitalPinToInterrupt(topSwitchIV), goDownIV, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bttmSwitchIV), goUpIV, CHANGE);
}

void loop(){
  switch(stage){
    case 'I':
      digitalWrite(trigPinV, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPinV, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPinV, LOW);
      durationV = pulseIn(echoPinV, HIGH);
      distanceV = durationV * 0.034 / 2;
      delay(100);   //DELAY IS NEEDED TO NOT READ 0
      if(distanceV <= 13 && distanceV > 9){
        Serial.println("going into stage V");
        stage = 'V';
      }
      break;
    case 'V':  
      Serial.print("Count: ");
      Serial.println(count);
      if(count % 2 == 0){   //raise outServo for g1
        outServo.write(10);
        delay(1000);
        stage = 'I';
      }
      else{
        delay(1000);
        outServo.write(65);
        delay(1000);
        stage = 'O';
      }
      visionServo.write(150);
      delay(1000);
      visionServo.write(60); // move arm back to original pos
      delay(1000);
      count++;
      break;  
    case 'O':
      digitalWrite(trigPinO, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPinO, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPinO, LOW);
      durationO = pulseIn(echoPinO, HIGH);
      distanceO = durationO * 0.034 / 2;
      delay(100);   //DELAY IS NEEDED TO NOT READ 0
      Serial.println("Waiting for lift");
      if(distanceO <= 12  && digitalRead(in4_1) == LOW){
        outServo.write(120);
        delay(2000);
        digitalWrite(in3_1, HIGH);
        digitalWrite(in4_1, LOW);
        outServo.write(55);
        delay(2000);
        //Serial.println("going into stage I");
        stage = 'I';
      }
      break;
    default:
      break;
    delay(100);
  }
}

void goUpIV(){
  if(downIV){
    digitalWrite(in3_2, HIGH);
    digitalWrite(in4_2, LOW);
    upIV = true;
    downIV = false;
  }
}

void goDownIV(){
  if(upIV){
    digitalWrite(in3_2, LOW);
    digitalWrite(in4_2, HIGH);
    downIV = true;
    upIV = false;
  } 
}

void stop(){
  if(downLeft){
    digitalWrite(in3_1, LOW);
    digitalWrite(in4_1, LOW);
    downRight = false;
    upRight = true;
    //Serial.println("stop Left");
    detachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift));
  }

}

void goDownLeft(){
  if(upLeft){
    digitalWrite(in3_1, LOW);
    digitalWrite(in4_1, HIGH);
    downRight = true;
    upRight = false;
    //Serial.println("down Left");
    attachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift), stop, CHANGE);
  }
 
}

void goUpRight(){
  if(downRight){
    digitalWrite(in1_1, HIGH);
    digitalWrite(in2_1, LOW);
    upRight = true;
    downRight = false;
    Serial.println("up Right");
  }
}

void goDownRight(){
  if(upRight){
    digitalWrite(in1_1, LOW);
    digitalWrite(in2_1, HIGH);
    downRight = true;
    upRight = false;
    Serial.println("down Right");
  }
}