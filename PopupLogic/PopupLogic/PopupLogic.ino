#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40); ;

const int buttonPin = 2;  //D2
bool buttonPressed = false;

struct Popup {
  static const int minAngle = 150;
  static const int maxAngle = 300;
  static const int dAngle = 5;
  static const int delayMs = 8;
  static const int maxButtonPresses = 2;

  int registeredPin;
  
  int moving;
  int poppedUp;
  int currentAngle;

  int unhandledPresses = 0;
  int wink = 0;

  Popup(int _pin) {
    registeredPin = _pin;

    moving = 0;
  }

  void ResetPositions() {
    pwm.setPWM(registeredPin, 0, minAngle);
    currentAngle = minAngle;
    poppedUp = 0;
  }

  void RegisterButtonPress() {
    if (unhandledPresses < maxButtonPresses)
      unhandledPresses++;
  }

  void Tick() {
    if (moving) {
      if (poppedUp ^ wink)
        MoveServoDown();
      else
        MoveServoUp();
    } else {
      if (unhandledPresses > 0) {
        moving = 1;
        unhandledPresses--;

        if (registeredPin) {
          if (unhandledPresses > 0) {
            wink = 1;
          } else {
            if (wink) {
              wink = 0;
              moving = 0;
            }
          }
        }
      }
    }
  }

  void MoveServoUp() {
    if (currentAngle < maxAngle) { //if there was more movement left in this period
      currentAngle += dAngle; //set new angle to move to
      if (currentAngle > maxAngle) //don't allow more than the maximum
        currentAngle = maxAngle;

      pwm.setPWM(registeredPin, 0, currentAngle);
    } else {
      moving = false;
      poppedUp = 1;
    }
  }

  void MoveServoDown() {
    if (currentAngle > minAngle) { //if there was more movement left in this period
      currentAngle -= dAngle; //set new angle to move to
      if (currentAngle < minAngle) //don't allow less than the minimum
        currentAngle = minAngle;

      pwm.setPWM(registeredPin, 0, currentAngle); 
    } else {
      moving = false;
      poppedUp = 0;
    }
  }
};

Popup p1 = Popup(0);
Popup p2 = Popup(1);


void setup() {
  Serial.begin(9600);

  Wire.begin();       // Start I2C communication
  pwm.begin();        // Start the servo controller
  pwm.setPWMFreq(50); // Set frequency to 50Hz for servos

  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor

  p1.ResetPositions();
  p2.ResetPositions();
}

void loop() {

  if (digitalRead(buttonPin) == LOW) { // Button is pressed (LOW due to pull-up)
    if (!buttonPressed) { 
      buttonPressed = true;
      
      p1.RegisterButtonPress();
      p2.RegisterButtonPress();
    }
  } else {
      buttonPressed = false; // Reset trigger when button is released
      delay(10);
  }

  p1.Tick();
  p2.Tick();

  delay(Popup::delayMs);
}
