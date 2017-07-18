/* Pin number constants*/

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

// Amount of time to flash the LEDs
const int ledDelayMS = 50;

// Amount of time to turn before resuming previous direction
const int turnDelayMS = 400;

// Received Bluetooth message
String receivedBuffer = "";

// Indicates that a message in the serial output stream is intended for
// another Bluetooth device, not for logging
String bluetoothOutputPrefix = "BL: ";

// Current movement direction
bool goingForward = false;
bool goingBackward = false;

const double soundVelocityMPS = 340.0;




/* LED control */

void flashGreenLED() {
  digitalWrite(greenLEDPin, HIGH);
  delay(ledDelayMS);
  digitalWrite(greenLEDPin, LOW);
}

void flashRedLED() {
  digitalWrite(redLEDPin, HIGH);
  delay(ledDelayMS);
  digitalWrite(redLEDPin, LOW);
}


/* Left motor control */

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


/* Right motor control */

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


/* Stopping and acceleration */

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


/* Turning */
// Briefly turn (adjust angle), then resume previous acceleration direction

void left() {
  flashGreenLED();
  
  if (goingForward) {
    leftStop();
    rightForward();
    delay(turnDelayMS);
    forward();
    
  } else if (goingBackward) {
    leftStop();
    rightBackward();
    delay(turnDelayMS);
    backward();
  }
}

void right() {
  flashGreenLED();

  if (goingForward) {
    leftForward();
    rightStop();
    delay(turnDelayMS);
    forward();
    
  } else if (goingBackward) {
    leftBackward();
    rightStop();
    delay(turnDelayMS);
    backward();
  }
}


/* Distance sensor */

// Sends an ultrasonic signal and measures the time for the echo signal
// to return
unsigned long readTimeUS() {
  // Activate the trigger pin to send the signal
  digitalWrite(sonarTrigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(sonarTrigPin, LOW);

  // Measure the time taken for the signal to return
  // (when the echo pin goes high)
  return pulseIn(sonarEchoPin, HIGH);
}

// Converts return time (us) to object distance (m) using the speed of sound
double timeUSToDistanceM(unsigned long timeUS) {
  // Must divide by 2 because the signal goes to the object and back (2 ways),
  // but the distance desired is only to the object (1 way)
  return ((double) timeUS * 1.0e-6) / 2.0 * soundVelocityMPS;
}


/* Bluetooth */

// Handles a received Bluetooth command
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




/* Main */

void setup() {
  // Start serial monitor
  Serial.begin(9600);
  Serial.println("Start");

  // Set up pins
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

  // Enable both motors
  digitalWrite(enableRPin, HIGH);
  digitalWrite(enableLPin, HIGH);

  stopMoving();

  // For logging distance data
//  Serial.println("Time (us)\tDistance (m)");
}


void loop() {
  // Receive Bluetooth messages
  if (Serial.available() > 0) {
    char received = Serial.read();

    // If the '\0' delimiter is received to indicate the end of the message,
    // handle the command
    if (received == '\0') {
      if (receivedBuffer != "") {
        Serial.println(receivedBuffer);
        
        handleCommand(receivedBuffer);
        receivedBuffer = "";
      }

    // If the delimiter is not received, add the character to the message
    } else {
      receivedBuffer += received;
    }
  }


  // Measure the forward distance of the closest object
  // Get data from distance sensor and convert it to a distance
  unsigned long timeUS = readTimeUS();
  double distanceM = timeUSToDistanceM(timeUS);
  double distanceCM = distanceM * 100;

  // Filter out false readings
  if (15 <= distanceCM && distanceCM < 300) {
    // Write the distance to Bluetooth output
    Serial.print(bluetoothOutputPrefix);
    Serial.println((uint8_t) distanceCM);
    
    // Log the distance data
//    Serial.print(timeUS);
//    Serial.print("\t\t");
//    Serial.println(distanceM);

    // If the robot is moving forward and within 25cm of an object, stop moving
    if (distanceCM < 25 && goingForward) {
      stopMoving();
    }

    // Give time to process the serial output stream
    delay(100);
  }
}




