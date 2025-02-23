#include <Servo.h>

bool run = true;
int timesRan = 0;
int timesToRun = 5;


Servo s; 
int servoPin = 9; //D9

const int buttonPin = 2;  //D2
bool buttonPressed = false;

void setup() {
  s.attach(servoPin);
  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor
}


  
void loop() {
  while (run) {
    MoveServoUp();
    delay(500);
    MoveServoDown();
    delay(500);

    timesRan++;
    if (timesRan >= timesToRun)
    run = false;
  }
}

void MoveServoUp() {
  for (int i = 0; i <= 180; i ++) {
    s.write(i);
    delay(5);
  }
}

void MoveServoDown() {
  for (int i = 180; i >= 0; i --) {
    s.write(i);
    delay(5);
  }
}

void Oscillate() {
  for (int i = 0; i <= 360; i ++) {
    if (i <= 180)
      s.write(i);
    else
      s.write(360 - i);
    delay(5);
  }
}