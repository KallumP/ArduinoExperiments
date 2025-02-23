#include <Servo.h>

Servo s; 
int servoPin = 9; //D9
int servoPosition = 0;

const int buttonPin = 2;  //D2
bool buttonPressed = false;

int delayMs = 4;
int maxAngle = 40;
int offset = 0;

void setup() {
  s.attach(servoPin);
  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor
  s.write(offset);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) { // Button is pressed (LOW due to pull-up)
    if (!buttonPressed) { 
      buttonPressed = true;
      
      if (servoPosition)
        MoveServoDown();
      else
        MoveServoUp();
    }
  } else {
      buttonPressed = false; // Reset trigger when button is released
      delay(10);
  }
}

void MoveServoUp() {
  for (int i = 0; i <= maxAngle; i++) {
    s.write(i + offset);
    delay(delayMs);
  }
  servoPosition = 1;
}
void MoveServoDown() {
  for (int i = maxAngle; i >= 0; i--) {
    s.write(i + offset);
    delay(delayMs);
  }
  servoPosition = 0;
}