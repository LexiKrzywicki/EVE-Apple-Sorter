//this is G1 arduino
#include <Servo.h>

const int trigPinV = 9;
const int echoPinV = 10;
const int trigPinO = 3;
const int echoPinO = 4;
long durationV;
int distanceV = 30;
long durationO;
int distanceO;
char readByte = '0';
bool run = false;
bool runG1 = false;

Servo myServo;
Servo servo2;

void setup(){
  pinMode(trigPinV, OUTPUT);
  pinMode(echoPinV, INPUT);
  myServo.attach(11);
  servo2.attach(2);
  pinMode(trigPinO, OUTPUT);
  pinMode(echoPinO, INPUT);
  myServo.write(95);
  delay(25);
  servo2.write(0);
  delay(25);
  Serial.begin(9600);
}

// void loop(){
//   digitalWrite(trigPinV, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPinV, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPinV, LOW);
//   durationV = pulseIn(echoPinV, HIGH);
//   distanceV = durationV * 0.034 / 2;
//   delay(100);   //DELAY IS NEEDED TO NOT READ 0
//   Serial.println(distanceV);
// }


void loop(){

  if(Serial.available()){
    readByte = Serial.read();

      switch(readByte){
        case 'A':
          run = true;   // for vision servo
          digitalWrite(trigPinV, LOW);
          delayMicroseconds(2);
          digitalWrite(trigPinV, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPinV, LOW);
          durationV = pulseIn(echoPinV, HIGH);
          distanceV = durationV * 0.034 / 2;
          delay(100);   //DELAY IS NEEDED TO NOT READ 0
          if(distanceV <= 13 && distanceV > 5){   //my need to get an average of values
            Serial.write("Z");
            //Serial.write(distanceCm);
          }
          break;
        case 'B':    // vision servo
          Serial.flush();
          if(run){   //makes sure this only runs once
            myServo.write(30);
            delay(2000);
            myServo.write(90);
            delay(1000);
            run = false;
            runG1 = false;
          }
          Serial.write("Y");  //prints when the servo is done moving
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
          if(distanceO < 8 && runG1 == false){  //if distance detected, servo runs
            servo2.write(180);
            delay(1000);
            servo2.write(0);
            delay(500);
            Serial.write("W");
            runG1 = true;
          }
          Serial.write("X");
        default:
          break;
        delay(100);
      }
  }
  
}




