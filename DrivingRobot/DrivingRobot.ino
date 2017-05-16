const int enable1Pin = 10;
const int enable2Pin = 9;
const int motor1Pin1 = 3;
const int motor1Pin2 = 4;
const int motor2Pin1 = 6;
const int motor2Pin2 = 7;

void setup() {
  Serial.print("Start");

  pinMode(enable1Pin, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  

  digitalWrite(enable1Pin, HIGH);
  digitalWrite(enable2Pin, HIGH);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void loop() {
}

