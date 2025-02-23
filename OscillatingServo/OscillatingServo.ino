#include <Servo.h>

bool on = false;

Servo s; 
int servoPin = 9; //D9

const int buttonPin = 2;  //D2
bool buttonPressed = false;

int delayMs = 20;
int maxAngle = 40;

void setup() {
  Serial.begin(9600);
  s.attach(servoPin);
  s.write(0);
  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor
}

void loop() {
  if (on) {
    Oscillate();
    delay(100);
  } else {
    CheckButtonPress();
  }
}

void CheckButtonPress() {
  if (digitalRead(buttonPin) == LOW) { // Button is pressed (LOW due to pull-up)
    if (!buttonPressed) { 
      buttonPressed = true;
      on = !on;
      Serial.println("Button pressed! switching circuit...");
    }
  } else {
      buttonPressed = false; // Reset trigger when button is released
      delay(10);
  }
}

void Oscillate() {
  for (int i = 0; i < maxAngle * 2; i++) {

    int angleToWrite = i;

    if (i > maxAngle) 
      angleToWrite = maxAngle * 2 - i;

    s.write(angleToWrite);
    delay(delayMs);

    CheckButtonPress();
  }
}