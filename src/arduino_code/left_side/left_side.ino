//left side

#include <Servo.h>

// #define enA 11
// #define in1 12
// #define in2 13

// #define enB 6
// #define in3 4
// #define in4 5

int topSwitch = 29;
int bttmSwitch = 31;
int enB = 13;
int in3 = 12;
int in4 = 11;

int motorPWM = 255;
bool goingUp = true;
bool goingDown = false;

const int trigPinV = 9;
const int echoPinV = 8;

long duration;
int distance;

Servo visionServo;
Servo outServo;

void setup() {
  // put your setup code here, to run once:
  // pinMode(trigPinV, OUTPUT);
  // pinMode(echoPinV, INPUT);
  // Serial.begin(9600);
  // visionServo.attach(7);
  // visionServo.write(90);

  // outServo.attach(2);
  // outServo.write(10);

  // pinMode(enA, OUTPUT);
  // pinMode(in1, OUTPUT);
  // pinMode(in2, OUTPUT);
  // digitalWrite(in1,LOW);
  // digitalWrite(in2, HIGH);
  // analogWrite(enA, 255); 

  // pinMode(enB, OUTPUT);
  // pinMode(in3, OUTPUT);
  // pinMode(in4, OUTPUT);
  // digitalWrite(in3,LOW);
  // digitalWrite(in4, HIGH);
  // analogWrite(enB, 255); 

  initLifter();

  delay(100);

}

void initLifter(){
  pinMode(topSwitch, INPUT_PULLUP);
  pinMode(bttmSwitch, INPUT_PULLUP);
  pinMode(enB, INPUT);
  pinMode(in3, INPUT);
  pinMode(in4, INPUT);  

  digitalWrite(enB, motorPWM);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(bttmSwitch) && goingDown){
    Serial.println("bttm");
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);  
    goingUp = true; goingDown = false;  
  }
  // lifter is going up and it triggers the switch for the top
  if(digitalRead(topSwitch) && goingUp){
    Serial.println("top");
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    goingUp = false; goingDown = true;
  }
}
