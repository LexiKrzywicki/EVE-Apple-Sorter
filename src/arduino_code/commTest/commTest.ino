const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 11;
long duration;
int distanceCm = 30;

void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  delay(25);
  Serial.begin(9600);
}

void loop(){
  char readByte = 0;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * 0.034 / 2;
    Serial.write(distanceCm);

  if(Serial.available() && distanceCm < 20){

    readByte = Serial.read();

    switch(readByte){
      case 'A':
        digitalWrite(ledPin, HIGH);
        delay(1000);
        digitalWrite(ledPin, LOW);
        break;
      case 'B':
        digitalWrite(ledPin, HIGH);
        delay(4000);
        digitalWrite(ledPin, LOW);
        break;
      default:
        break;
    }
  }


  // digitalWrite(trigPin, LOW);
  // delayMicroseconds(2);
  // digitalWrite(trigPin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigPin, LOW);
  // duration = pulseIn(echoPin, HIGH);
  // distanceCm = duration * 0.034 / 2;
  // Serial.println(distanceCm);

  // if(distanceCm < 15){
  //   digitalWrite(ledPin, HIGH);
  // }
  // else{
  //   digitalWrite(ledPin, LOW);
  // }
  // delay(100);
}

