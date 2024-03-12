//this is G1 arduino
#include <Servo.h>

#define enA 13
#define in1 12
#define in2 11

const int trigPinV = 9;
const int echoPinV = 8;
const int trigPinO = 5;
const int echoPinO = 4;
long durationV;
int distanceV = 30;
long durationO;
int distanceO;
char readByte = '0';
bool run = false;
bool runG1 = false;            

Servo visionServo;
Servo outServo;

void setup(){
  //pinMode(hallSensorPin, INPUT)
  pinMode(trigPinV, OUTPUT);
  pinMode(echoPinV, INPUT);
  visionServo.attach(7);
  outServo.attach(2);
  pinMode(trigPinO, OUTPUT);
  pinMode(echoPinO, INPUT);
  visionServo.write(95);
  delay(25);
  outServo.write(105);
  delay(25);
  Serial.begin(9600);
  
  //converyor motor
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 255); 
  delay(100);
}


void loop(){

  if(Serial.available()){
    readByte = Serial.read();

      switch(readByte){
        case 'A':
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
            Serial.write("Z");
            //noInterrupts();  //disable interrupts for motor to start running again
            digitalWrite(in1, LOW);  //may need to change directions
            digitalWrite(in2, HIGH);
            //Serial.write(distanceCm);
          }
          break;
        case 'B':    // vision servo
          //Serial.flush();
          if(run){   //makes sure this only runs once
            visionServo.write(150);  //right vision servo
            delay(2000);
            visionServo.write(90);
            delay(1000);
            run = false;
            runG1 = true;
            Serial.write("Y");
          }
          //Serial.write("Y");  //prints when the servo is done moving
          break;
        case 'C':  //for G1servo
        //180 is up to the left this should be the end pos
        //should start at 0
          digitalWrite(trigPinO, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPinO, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPinO, LOW);
          durationO = pulseIn(echoPinO, HIGH);
          distanceO = durationO * 0.034 / 2;
          delay(100);   //DELAY IS NEEDED TO NOT READ 0
          Serial.println(distanceO);
          if(distanceO < 8 && runG1){  //if distance detected, servo runs
            outServo.write(0);
            delay(2000);
            outServo.write(105);
            delay(500);
            Serial.write("W");
            runG1 = false;
            //noInterrupts();
            //digitalWrite(in1, HIGH);  //used to run the motor up not sure about direction
            //digitalWrite(in2, LOW);
          }
          Serial.write("X");
        default:
          break;
        delay(100);
      }
  }
}




