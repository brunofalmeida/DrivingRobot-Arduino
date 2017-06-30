const int enableLPin = 8;
const int enableRPin = 13;

const int motorLPin1 = 9;
const int motorLPin2 = 10;
const int motorRPin1 = 12;
const int motorRPin2 = 11;

const int greenLEDPin = 2;
const int redLEDPin = 3;

const int sonarEchoPin = 5;
const int sonarTrigPin = 4;

const int ledDelay = 50;
const int turnDelay = 400;

String receivedBuffer = "";

bool goingForward = false;
bool goingBackward = false;

const double soundVelocityMPS = 340.0;



void flashGreenLED() {
  digitalWrite(greenLEDPin, HIGH);
  delay(ledDelay);
  digitalWrite(greenLEDPin, LOW);
}

void flashRedLED() {
  digitalWrite(redLEDPin, HIGH);
  delay(ledDelay);
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

  goingForward = false;
  goingBackward = false;
}

void forward() {
  leftForward();
  rightForward();
  flashGreenLED();

  goingForward = true;
  goingBackward = false;
}

void backward() {
  leftBackward();
  rightBackward();
  flashGreenLED();

  goingForward = false;
  goingBackward = true;
}

void left() {
  flashGreenLED();

  // Briefly turn left, then resume previous direction
  if (goingForward) {
    leftStop();
    rightForward();
    delay(turnDelay);
    forward();
    
  } else if (goingBackward) {
    leftStop();
    rightBackward();
    delay(turnDelay);
    backward();
  }
}

void right() {
  flashGreenLED();

  // Briefly turn right, then resume previous direction
  if (goingForward) {
    leftForward();
    rightStop();
    delay(turnDelay);
    forward();
    
  } else if (goingBackward) {
    leftBackward();
    rightStop();
    delay(turnDelay);
    backward();
  }
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

  pinMode(sonarEchoPin, INPUT);
  pinMode(sonarTrigPin, OUTPUT);

  digitalWrite(enableRPin, HIGH);
  digitalWrite(enableLPin, HIGH);

  stopMoving();


  // Test the sonar ultrasonic sensor
  Serial.println("Time (us)\t\tDistance (m)");

  while (true) {
    // Activate the trigger pin to send the signal
    digitalWrite(sonarTrigPin, HIGH);
    delayMicroseconds(15);
    digitalWrite(sonarTrigPin, LOW);

    // Measure the time taken for the echo signal to return
    unsigned long timeUS = pulseIn(sonarEchoPin, HIGH);
    
    // Calculate the distance to the object
    double distanceM = ((double) timeUS * 1.0e-6) / 2.0 * soundVelocityMPS;

    // Filter out false readings
    if (timeUS < 1e5) {
      Serial.print(timeUS);
      Serial.print("\t\t");
      Serial.println(distanceM);
    }
    
    delay(1000);
  }
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




