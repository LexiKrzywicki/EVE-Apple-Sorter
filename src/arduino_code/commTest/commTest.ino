const int trigPin = 9;
const int echoPin = 10;
const int ledPin = 11;
long duration;
int distanceCm = 30;
char readByte = '0';

void setup(){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  delay(25);
  Serial.begin(9600);
}

void loop(){
  //char readByte = '0';

  //if(Serial.available()){
    readByte = Serial.read();

    switch(readByte){
      case 'A':
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        duration = pulseIn(echoPin, HIGH);
        distanceCm = duration * 0.034 / 2;
        if(distanceCm < 20){
          Serial.write("P");
        }
        break;
      case 'B':
        Serial.flush();
        digitalWrite(ledPin, HIGH);
        delay(1000);
        digitalWrite(ledPin, LOW);
        delay(100);
        while(Serial.read() != 'A'){
          Serial.write("D");
        }
          
        //Serial.write("D");
        // Serial.flush();
        // if(true){
        //   Serial.write("D"); //indicates the servo has finished moving
        // }
        break;
      default:
        break;
    }
  //}
}


// void loop(){
//   char readByte = 0;
//     digitalWrite(trigPin, LOW);
//     delayMicroseconds(2);
//     digitalWrite(trigPin, HIGH);
//     delayMicroseconds(10);
//     digitalWrite(trigPin, LOW);
//     duration = pulseIn(echoPin, HIGH);
//     distanceCm = duration * 0.034 / 2;
//     //Serial.println(distanceCm);

//     if(distanceCm < 20){
//       Serial.write("P");
//     }

//   if(Serial.available()){

//     readByte = Serial.read();

//     switch(readByte){
//       case 'A':
//         digitalWrite(ledPin, HIGH);
//         delay(1000);
//         digitalWrite(ledPin, LOW);
//         break;
//       case 'B':
//         digitalWrite(ledPin, HIGH);
//         delay(4000);
//         digitalWrite(ledPin, LOW);
//         break;
//       default:
//         break;
//     }
//   }


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
// }

