//right side - L1

#include <Servo.h>

//motor driver
int enA = 8;
int in1 = 9;
int in2 = 10;
int in3 = 11;
int in4 = 12;
int enB = 13;

//for lifter
int topSwitch = 21;
int bttmSwitch = 20;

//ultrasonics
const int trigPinV = 6;
const int echoPinV = 5;
const int trigPinO = 4;
const int echoPinO = 3;
long durationV;
int distanceV = 30;
long durationO;
int distanceO = 30;


char readByte = '0';
bool run = false;
bool runG1 = false;  
bool once = true;
bool upIV = false;
bool downIV = true;  

Servo visionServo;
Servo outServo;


char stage = 'I';
int count = 0;

void setup(){
  //ultrasonics
  pinMode(trigPinV, OUTPUT);
  pinMode(echoPinV, INPUT);
  visionServo.attach(7);
  outServo.attach(2);
  pinMode(trigPinO, OUTPUT);
  pinMode(echoPinO, INPUT);
  visionServo.write(60);
  delay(25);
  outServo.write(100);
  delay(25);
  Serial.begin(9600);

  //lifter motor
  pinMode(topSwitch, INPUT_PULLUP);
  pinMode(bttmSwitch, INPUT_PULLUP);
  pinMode(enB, INPUT);
  pinMode(in3, INPUT);
  pinMode(in4, INPUT);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  upIV = true;
  downIV = false;

  analogWrite(enB, 255);

  //conveyor
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 255);

  attachInterrupt(digitalPinToInterrupt(topSwitch), goDownIV, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bttmSwitch), goUpIV, CHANGE);

  delay(1000);

  Serial.println("setup complete");

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
      // Serial.println(distanceV);
      // Serial.println(stage);
      if(distanceV <= 13 && distanceV > 9){
        // Serial.print("Distance: ");
        // Serial.println(distanceV);
        Serial.println("going into stage V");
        stage = 'V';
      }
      break;
    case 'V':
      Serial.print("Count: ");
      Serial.println(count);
      if(count % 2 == 0){   //raise outServo for g2
        //delay(1000);
        outServo.write(100);
        delay(1000);
        // Serial.println("going into stage I");
        stage = 'I';
      }
      else{
        delay(1000);
        outServo.write(65);
        delay(1000);
        // Serial.println("going into stage O");
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
      if(distanceO <= 12){
        outServo.write(0);
        delay(2000);
        outServo.write(100);
        delay(2000);
        Serial.println("going into stage I");
        stage = 'I';
      }
      break;
    default:
      break;
    delay(1000);
  }
}

void goUpIV(){
  if(downIV){
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    upIV = true;
    downIV = false;
  }
}

void goDownIV(){
  if(upIV){
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    downIV = true;
    upIV = false;
  } 
}

void stop(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
