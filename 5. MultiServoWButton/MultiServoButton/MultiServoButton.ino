#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40); ;

const int buttonPin = 2;  //D2
bool buttonPressed = false;

struct PopUp {
  static const int minAngle = 150;
  static const int maxAngle = 300;
  static const int dAngle = 5;
  static const int delayMs = 5;

  int registeredPin;
  
  int moving;
  int poppedUp;
  int currentAngle;

  PopUp(int _pin) {
    registeredPin = _pin;

    moving = 0;
  }

  void ResetPositions() {
    pwm.setPWM(registeredPin, 0, minAngle);
    currentAngle = minAngle;
    poppedUp = 0;
  }

  void RegisterButtonPress() {
    moving = 1;
  }

  void Tick() {
    if (moving) {
      if (poppedUp)
        MoveServoDown();
      else
        MoveServoUp();
    }
  }

  void MoveServoUp() {
    if (currentAngle < PopUp::maxAngle) { //if there was more movement left in this period
      currentAngle += dAngle; //set new angle to move to
      if (currentAngle > PopUp::maxAngle) { //don't allow more than the maximum
        currentAngle = PopUp::maxAngle;
        Serial.print("Had to constrain to maximum for: ");
        Serial.println(registeredPin);
      }

      pwm.setPWM(registeredPin, 0, currentAngle);
      delay(PopUp::delayMs);

    } else {
      moving = false;
      poppedUp = 1;
    }
  }

  void MoveServoDown() {
    if (currentAngle > PopUp::minAngle) { //if there was more movement left in this period
      currentAngle -= dAngle; //set new angle to move to
      if (currentAngle < PopUp::minAngle) { //don't allow less than the minimum
        currentAngle = PopUp::minAngle;
        Serial.print("Had to constrain to minimum for: ");
        Serial.println(registeredPin);
      }

      pwm.setPWM(registeredPin, 0, currentAngle); 
      delay(PopUp::delayMs);

    } else {
      moving = false;
      poppedUp = 0;
    }
  }
};

PopUp p1 = PopUp(0);
PopUp p2 = PopUp(1);

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
}
