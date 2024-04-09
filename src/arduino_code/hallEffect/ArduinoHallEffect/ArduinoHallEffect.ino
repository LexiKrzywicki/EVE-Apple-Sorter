int topSwitchIV = 21;
int bttmSwitchIV = 20; 

int topSwitchRightLift = 3;
int bttmSwitchRightLift = 2;

int topSwitchLeftLift = 19;
int bttmSwitchLeftLift = 18;

//ultrasonics
const int trigPinV = 47;
const int echoPinV = 49;
const int trigPinO = 7;
const int echoPinO = 6;
long durationV;
int distanceV = 30;
long durationO;
int distanceO;

//motor driver 1 - left and right lifters
int enA_1 = 8;   //right
int in1_1 = 9;   
int in2_1 = 10;
int in3_1 = 11;  //left
int in4_1 = 12;
int enB_1 = 13;

//motor driver 2 - conveyor and I->V lifter
int enA_2 = 35;  //conveyor
int in1_2 = 37;
int in2_2 = 39;
int in3_2 = 41;   //lift
int in4_2 = 43;
int enB_2 = 45;

bool upRight = false;
bool downRight = true;
bool downLeft = true;
bool upLeft = false;
bool upIV = false;
bool downIV = true;

void setup() {

  Serial.begin(9600);
  //right lift
  pinMode(topSwitchRightLift, INPUT_PULLUP);
  pinMode(bttmSwitchRightLift, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(topSwitchRightLift), goDownRight, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(bttmSwitchRightLift), goUpRight, CHANGE);
  pinMode(enA_1, INPUT);
  pinMode(in1_1, INPUT);
  pinMode(in2_1, INPUT);
  digitalWrite(in1_1, HIGH);
  digitalWrite(in2_1, LOW);
  upRight = true;
  downRight = false;
  analogWrite(enA_1, 255);


  pinMode(enA_2, OUTPUT);
  pinMode(in1_2, OUTPUT);
  pinMode(in2_2, OUTPUT);
  digitalWrite(in1_2, HIGH);
  digitalWrite(in2_2, LOW);
  analogWrite(enA_2, 255);

  //left lift
  pinMode(topSwitchLeftLift, INPUT_PULLUP);
  pinMode(bttmSwitchLeftLift, INPUT_PULLUP);
  // // attachInterrupt(digitalPinToInterrupt(topSwitchLeftLift), goDownLeft, CHANGE);
  // // attachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift), stop, CHANGE);
  // pinMode(enB_1, INPUT);
  // pinMode(in3_1, INPUT);
  // pinMode(in4_1, INPUT);
  // digitalWrite(in3_1, LOW);
  // digitalWrite(in4_1, HIGH);
  // upRight = false;
  // downRight = true;
  // analogWrite(enB_1, 255);

  attachInterrupt(digitalPinToInterrupt(topSwitchRightLift), goDownRight, RISING);
  attachInterrupt(digitalPinToInterrupt(bttmSwitchRightLift), goUpRight, RISING);
  attachInterrupt(digitalPinToInterrupt(topSwitchLeftLift), goDownLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(bttmSwitchLeftLift), goUpLeft, CHANGE);
}


// Code will have the lifter going up first
void loop() {
}

void goUpRight(){
  if(downRight){
    digitalWrite(in1_1, HIGH);
    digitalWrite(in2_1, LOW);
    upRight = true;
    downRight = false;
    Serial.println("going up");
  }
}

void goDownRight(){
  if(upRight){
    digitalWrite(in1_1, LOW);
    digitalWrite(in2_1, HIGH);
    downRight = true;
    upRight = false;
    Serial.println("going down");
  }
}


void goUpLeft(){
  if(downLeft){
    digitalWrite(in3_1, HIGH);
    digitalWrite(in4_1, LOW);
    downLeft = false;
    upLeft = true;
    Serial.println("going up");
  }
}

void goDownLeft(){
  if(upLeft){
    digitalWrite(in3_1, LOW);
    digitalWrite(in4_1, HIGH);
    downLeft = true;
    upLeft = false;
    Serial.println("going down");
  }
}