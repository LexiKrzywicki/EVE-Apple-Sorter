/*
Arduino Hall Effect Sensor Project
by Arvind Sanjeev
Please check out  http://diyhacking.com for the tutorial of this project.
DIY Hacking
*/

#define enA 9
#define in1 6
#define in2 7

 volatile byte half_revolutions;
 unsigned int rpm;
 unsigned long timeold;

 bool clockwise;

 void setup()
 {
   Serial.begin(115200);
   attachInterrupt(0, magnet_detect_top, RISING);//Initialize the intterrupt pin (Arduino digital pin 2)
   attachInterrupt(1, magnet_detect_bottom, FALLING);//Initialize the intterrupt pin (Arduino digital pin 2)
   half_revolutions = 0;
   rpm = 0;
   timeold = 0;
   pinMode(enA, OUTPUT);
   pinMode(in1, OUTPUT);
   pinMode(in2, OUTPUT);
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   clockwise = false;
   analogWrite(enA, 100); 
   delay(2000);
 }
 void loop()//Measure RPM
 {
   if (half_revolutions >= 20) { 
     rpm = 30*1000/(millis() - timeold)*half_revolutions;
     timeold = millis();
     half_revolutions = 0;
     //Serial.println(rpm,DEC);
   }
 }
 void magnet_detect_top()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   half_revolutions++;
   Serial.println("detect top");
   //Serial.println("clockwise");
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
 }

 void magnet_detect_bottom()
 {
  half_revolutions++;
  Serial.println("detect bottom");
  //Serial.println("counterclockwise");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
 }
