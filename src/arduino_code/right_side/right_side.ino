#include <Servo.h>

//motor driver
int enA = 8;
int in1 = 9;
int in2 = 10;
int in3 = 11;
int in4 = 12;
int enB = 13;

//for lifter
int topSwitch = 29;
int bttmSwitch = 31;

//ultrasonics
const int trigPinV = 6;
const int echoPinV = 5;
const int trigPinO = 4;
const int echoPinO = 3;

long durationV;
int distanceV = 30;
long durationO;
int distanceO;
char readByte = '0';
bool run = false;
bool runG1 = false;  
bool once = true;
bool goingUp = false;
bool goingDown = true;  

Servo visionServo;
Servo outServo;

void setup() {
  pinMode(trigPinV, OUTPUT);
  pinMode(echoPinV, INPUT);
  visionServo.attach(7);
  outServo.attach(2);
  pinMode(trigPinO, OUTPUT);
  pinMode(echoPinO, INPUT);
  visionServo.write(10);
  delay(25);
  outServo.write(100);
  delay(25);
  Serial.begin(9600);
  
  //converyor motor
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1,LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 255);

  //lifter motor
  pinMode(topSwitch, INPUT_PULLUP);
  pinMode(bttmSwitch, INPUT_PULLUP);
  attachInterrupt(29, stop, CHANGE);//Initialize the intterrupt pin (Arduino digital pin 2)
  attachInterrupt(31, stop, CHANGE);//Initialize the intterrupt pin (Arduino digital pin 2)
  pinMode(enB, INPUT);
  pinMode(in3, INPUT);
  pinMode(in4, INPUT);
  digitalWrite(in3,LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 255);

  delay(100);

}

void loop(){
    
  //lifter limit switches
  // if(digitalRead(bttmSwitch) && goingDown){
  //   Serial.println("bttm");
  //   digitalWrite(in3, LOW);
  //   digitalWrite(in4, HIGH);  
  //   goingUp = true; goingDown = false;  
  // }
  // // lifter is going up and it triggers the switch for the top
  // if(digitalRead(topSwitch) && goingUp){
  //   Serial.println("top");
  //   digitalWrite(in3, HIGH);
  //   digitalWrite(in4, LOW);
  //   goingUp = false; goingDown = true;
  // }

  if(Serial.available()){
    readByte = Serial.read();

      switch(readByte){
        case 'A':
        //run the lifter down
        if(digitalRead(bttmSwitch) && goingDown){
          Serial.println("top");
          digitalWrite(in3, LOW);
          digitalWrite(in4, LOW);
          goingUp = false; goingDown = true;
        }
        else{
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
        }
          run = true;   // used in vision servo case
          digitalWrite(trigPinV, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPinV, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPinV, LOW);
          durationV = pulseIn(echoPinV, HIGH);
          distanceV = durationV * 0.034 / 2;
          delay(100);   //DELAY IS NEEDED TO NOT READ 0
          if(distanceV <= 14 && distanceV > 5){   //may need to get an average of values
            Serial.write("Z");
          }
          break;
        case 'B':    // vision servo
          if(run){   //makes sure this only runs once
            visionServo.write(150);  //right vision servo
            delay(2000);
            visionServo.write(10);
            delay(1000);
            interrupts();
            run = false;
            runG1 = true;
            Serial.write("Y");
          }
          break;
        case 'C':  //for G1servo
        //180 is up to the left this should be the end pos
        //should start at 0
        if(once){
          outServo.write(65);
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
          if(distanceO == 12 && runG1){  //if distance detected, servo runs
            outServo.write(0);
            delay(2000);
            outServo.write(100);
            delay(500);
            Serial.write("W");
            runG1 = false;
            once = true;
          }
        case 'D':  //for g2
          outServo.write(100);
          delay(500);
          Serial.write("U");
        default:
          break;
        delay(100);
      }
  }
}

void stop(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
