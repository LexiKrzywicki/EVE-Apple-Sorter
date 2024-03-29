/*
Arduino Hall Effect Sensor Project
by Arvind Sanjeev
Please check out  http://diyhacking.com for the tutorial of this project.
DIY Hacking
*/

#include <Servo.h>

Servo servo;

int in3 = 11;
int in4 = 12;
int enB = 13;

int topSwitch = 21;
int bttmSwitch = 20;

 void setup()
 {
  //Serial.begin(115200);
  pinMode(topSwitch, INPUT_PULLUP);
  pinMode(bttmSwitch, INPUT_PULLUP);
  pinMode(enB, INPUT);
  pinMode(in3, INPUT);
  pinMode(in4, INPUT);
  //digitalWrite(in3, HIGH);
  //digitalWrite(in4, LOW);
  analogWrite(enB, 255);

  attachInterrupt(digitalPinToInterrupt(topSwitch), goDown, CHANGE);//Initialize the intterrupt pin (Arduino digital pin 2)
  attachInterrupt(digitalPinToInterrupt(bttmSwitch), goUp, CHANGE);//Initialize the intterrupt pin (Arduino digital pin 2)
  
  servo.attach(7);

  Serial.begin(9600);
  delay(500);
 }
 void loop()
 {  
    servo.write(15);
    delay(2000);
    servo.write(180);
    delay(2000);
 }

 void stop(){
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.println("STOPPED");
  delay(100);

 }
 void goUp()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);  
 }


void goDown()
 {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
 }
