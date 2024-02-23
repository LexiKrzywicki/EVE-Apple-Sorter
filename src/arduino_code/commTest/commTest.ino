#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distanceCm = 30;
char readByte = '0';
bool run = false;

Servo myServo;
int pos = 0;

void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(11);
  // pinMode(ledPin, OUTPUT);
  // digitalWrite(ledPin, LOW);
  myServo.write(90);
  delay(25);
  Serial.begin(9600);
}

void loop(){
  readByte = Serial.read();

  switch(readByte){
    case 'A':
      run = true;
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      duration = pulseIn(echoPin, HIGH);
      distanceCm = duration * 0.034 / 2;
      delay(100);   //DELAY IS NEEDED TO NOT READ 0
      if(distanceCm < 14){
        Serial.write("P");
        //Serial.write(distanceCm);
      }
      break;
    case 'B':
      Serial.flush();
      if(run){
        myServo.write(30);
        delay(2000);
        myServo.write(90);
        delay(1000);
        run = false;
      }
      Serial.write("D");
      break;
    default:
      break;
  }
}



// void loop(){
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(2);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
//   duration = pulseIn(echoPin, HIGH);
//   distanceCm = duration * 0.034 / 2;
//   Serial.println(distanceCm);

//   delay(100);
// }




