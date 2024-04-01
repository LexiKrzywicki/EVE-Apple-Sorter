//left side - L2

#include <Servo.h>


int topSwitchIV = 21;
int bttmSwitchIV = 20; 

int topSwitchRightLift = 18;
int bttmSwitchRightLift = 19;

int topSwitchLeftLift = 3;
int bttmSwitchLeftLift = 2;

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

char readByte = '0';
bool run = false;
bool runG1 = false;  
bool once = true;
bool goingUp = false;
bool goingDown = true;  

Servo visionServo;
Servo outServo;

void setup(){

  pinMode(trigPinV, OUTPUT);
  pinMode(echoPinV, INPUT);
  visionServo.attach(51);
  outServo.attach(53);
  pinMode(trigPinO, OUTPUT);
  pinMode(echoPinO, INPUT);
  visionServo.write(125);
  delay(25);
  outServo.write(35);
  delay(25);
  Serial.begin(9600);

  //I->V lift
  pinMode(topSwitchIV, INPUT_PULLUP);
  pinMode(bttmSwitchIV, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(topSwitchIV), goDownIV, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bttmSwitchIV), goUpIV, CHANGE);
  pinMode(enB_2, INPUT);
  pinMode(in3_2, INPUT);
  pinMode(in4_2, INPUT);
  digitalWrite(in3_2, HIGH);
  digitalWrite(in4_2, LOW);
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
  attachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift), stop, CHANGE);
  pinMode(enB_1, INPUT);
  pinMode(in3_1, INPUT);
  pinMode(in4_1, INPUT);
  digitalWrite(in3_1, LOW);
  digitalWrite(in4_1, HIGH);
  analogWrite(enB_1, 255);


  delay(1000);
}

void loop(){
if(Serial.available()){
    readByte = Serial.read();

      switch(readByte){
        case 'A':
          attachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift), stop, CHANGE);
          run = true;   // used in vision servo case
          digitalWrite(trigPinV, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPinV, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPinV, LOW);
          durationV = pulseIn(echoPinV, HIGH);
          distanceV = durationV * 0.034 / 2;
          delay(100);   //DELAY IS NEEDED TO NOT READ 0
          if(distanceV <= 13 && distanceV > 5){   //may need to get an average of values
            delay(500);
            Serial.write("Z");
            once = true;
          }
          break;
        case 'B':    // vision servo
          if(run){
            visionServo.write(10);  //move right vision servo
            delay(2000);

            //move servo back to original position for next apple
            visionServo.write(125);
            delay(1000);
            run = false;
            runG1 = true;
            Serial.write("Y");
          }
          break;
        case 'C':  //for G2servo
        //180 is up to the left this should be the end pos
        //should start at 0
        if(once){
          outServo.write(35);
          once = false;
        }
          digitalWrite(trigPinO, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPinO, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPinO, LOW);
          durationO = pulseIn(echoPinO, HIGH);
          distanceO = durationO * 0.034 / 2;
          delay(100);   //DELAY IS NEEDED TO NOT READ 0
          Serial.println(distanceO);
          if(distanceO <= 18 && runG1){  //if distance detected, servo runs
            // outServo.write(120);
            // delay(2000);
            // outServo.write(35);
            // delay(500);
            Serial.write("W");
            runG1 = false;
            once = true;

            digitalWrite(in3_1, HIGH);
            digitalWrite(in4_1, LOW);
          }
        break;
        case 'D':  //for g1
          outServo.write(10);
          delay(500);
          Serial.write("V");
        break;

        default:
          break;
        delay(100);
      }
  }
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

void stop(){
  digitalWrite(in3_1, LOW);
  digitalWrite(in4_1, LOW);

  dettachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift));
}

void goDownLeft(){
  digitalWrite(in3_1, LOW);
  digitalWrite(in4_1, HIGH);
}

