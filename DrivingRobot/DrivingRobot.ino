const int enable1Pin = 10;
const int enable2Pin = 9;

const int motorLPin1 = 6;
const int motorLPin2 = 7;
const int motorRPin1 = 3;
const int motorRPin2 = 4;

void leftForward() {
  digitalWrite(motorLPin1, HIGH);
  digitalWrite(motorLPin2, LOW);
}

void leftBackward() {
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, HIGH);
}

void rightForward() {
  digitalWrite(motorRPin1, HIGH);
  digitalWrite(motorRPin2, LOW);
}

void rightBackward() {
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, HIGH);
}




void setup() {
  Serial.begin(9600);
  Serial.print("Start");

  pinMode(enable1Pin, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  pinMode(motorLPin1, OUTPUT);
  pinMode(motorLPin2, OUTPUT);
  pinMode(motorRPin1, OUTPUT);
  pinMode(motorRPin2, OUTPUT);
  
  digitalWrite(enable1Pin, HIGH);
  digitalWrite(enable2Pin, HIGH);

  leftForward();
  rightForward();
}

void loop() {
  leftForward();
  delay(500);
  rightBackward();
  delay(500);
  leftBackward();
  delay(500);
  rightForward();
  delay(500);
}

