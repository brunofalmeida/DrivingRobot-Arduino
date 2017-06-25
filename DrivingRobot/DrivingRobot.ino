const int enableLPin = 9;
const int enableRPin = 10;

const int motorLPin1 = 6;
const int motorLPin2 = 7;
const int motorRPin1 = 4;
const int motorRPin2 = 3;

const int greenLEDPin = 2;
const int redLEDPin = 5;

String receivedBuffer = "";




void flashGreenLED() {
  digitalWrite(greenLEDPin, HIGH);
  delay(100);
  digitalWrite(greenLEDPin, LOW);
}

void flashRedLED() {
  digitalWrite(redLEDPin, HIGH);
  delay(100);
  digitalWrite(redLEDPin, LOW);
}


void leftStop() {
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, LOW);
}

void leftForward() {
  digitalWrite(motorLPin1, HIGH);
  digitalWrite(motorLPin2, LOW);
}

void leftBackward() {
  digitalWrite(motorLPin1, LOW);
  digitalWrite(motorLPin2, HIGH);
}


void rightStop() {
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, LOW);
}

void rightForward() {
  digitalWrite(motorRPin1, HIGH);
  digitalWrite(motorRPin2, LOW);
}

void rightBackward() {
  digitalWrite(motorRPin1, LOW);
  digitalWrite(motorRPin2, HIGH);
}


void stopMoving() {
  leftStop();
  rightStop();
  flashRedLED();
}

void forward() {
  leftForward();
  rightForward();
  flashGreenLED();
}

void backward() {
  leftBackward();
  rightBackward();
  flashGreenLED();
}

void left() {
  leftBackward();
  rightForward();
  flashGreenLED();
}

void right() {
  leftForward();
  rightBackward();
  flashGreenLED();
}


void handleCommand(String command) {
  if (command == "S") {
    stopMoving();
  } else if (command == "F") {
    forward();
  } else if (command == "B") {
    backward();
  } else if (command == "L") {
    left();
  } else if (command == "R") {
    right();
  }
}




void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  pinMode(enableLPin, OUTPUT);
  pinMode(enableRPin, OUTPUT);
  
  pinMode(motorLPin1, OUTPUT);
  pinMode(motorLPin2, OUTPUT);
  pinMode(motorRPin1, OUTPUT);
  pinMode(motorRPin2, OUTPUT);
  
  pinMode(greenLEDPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);

  digitalWrite(enableRPin, HIGH);
  digitalWrite(enableLPin, HIGH);

  stopMoving();
}


void loop() {
  // Receive Bluetooth signals
  if (Serial.available() > 0) {
    char received = Serial.read();
    
    if (received == '\0') {
      if (receivedBuffer != "") {
        Serial.println(receivedBuffer);
        handleCommand(receivedBuffer);
        receivedBuffer = "";
      }
      
    } else {
      receivedBuffer += received;
    }
  }
}




