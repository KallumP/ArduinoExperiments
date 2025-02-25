#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40); ;

const int buttonPin = 2;  //D2
bool buttonPressed = false;

struct PopUp {

  static const int minAngle = 150;
  static const int maxAngle = 300;
  static const int delayMs = 50;

  int pin;
  
  int servoPosition = 0;


  PopUp(int _pin) {
    pin = _pin;
    pwm.setPWM(pin, 0, minAngle);
  }

  void Toggle() {
    if (servoPosition)
      MoveServoDown();
    else
      MoveServoUp();
  }
  void MoveServoUp() {
    for (int i = PopUp::minAngle; i <= PopUp::maxAngle; i+= 50) {
      pwm.setPWM(pin, 0, i);
      delay(PopUp::delayMs);
    }
    servoPosition = 1;
  }
  void MoveServoDown() {
    for (int i = PopUp::maxAngle; i >= PopUp::minAngle; i-=50) {
      pwm.setPWM(pin, 0, i);
      delay(PopUp::delayMs);
    }
    servoPosition = 0;
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
}

void loop() {

  if (digitalRead(buttonPin) == LOW) { // Button is pressed (LOW due to pull-up)
    if (!buttonPressed) { 
      buttonPressed = true;
      
      p1.Toggle();
      p2.Toggle();
    }
  } else {
      buttonPressed = false; // Reset trigger when button is released
      delay(10);
  }
}
