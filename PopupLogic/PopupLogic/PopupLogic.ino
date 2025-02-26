#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40); ;
struct Popup {
  static const int minPosition = 150;
  static const int maxPosition = 300;
  static const int dPosition = 5;
  static const int delayMs = 8;
  static const int maxButtonPresses = 2;

  int registeredPin;

  int moving;
  int poppedUp;
  int currentPosition;

  int unhandledPresses = 0;

  Popup(int _pin) {
    registeredPin = _pin;
    moving = 0;
  }

  void SetPosition(int posisition) {
    int constrainedPosition = posisition;
    if (constrainedPosition > maxPosition)
      constrainedPosition = maxPosition;
    else if (constrainedPosition < minPosition)
      constrainedPosition = minPosition;

    pwm.setPWM(registeredPin, 0, constrainedPosition);
    currentPosition = constrainedPosition;
  }

  void FoceDown() {
    for (int i = currentPosition; i > minPosition; i -= dPosition) {
      SetPosition(i);
      delay(delayMs);
    }
    SetPosition(minPosition);
    poppedUp = 0;
  }

  void ForceUp() {
    for (int i = currentPosition; i < maxPosition; i += dPosition) {
      SetPosition(i);
      delay(delayMs);
    }
    SetPosition(maxPosition);
    poppedUp = 1;
  }

  void RegisterButtonPress() {
    if (unhandledPresses < maxButtonPresses)
      unhandledPresses++;
  }

  void Tick() {
    if (moving) {
      if (poppedUp)
        MoveServoDown();
      else
        MoveServoUp();
    } else {
      if (unhandledPresses > 0) {
        moving = 1;
        unhandledPresses--;
      }
    }
  }

  void MoveServoUp() {
    if (currentPosition < maxPosition) { //if there was more movement left in this period
      currentPosition += dPosition; //set new position to move to
      if (currentPosition > maxPosition) //don't allow more than the maximum
        currentPosition = maxPosition;
      SetPosition(currentPosition);
    } else {
      moving = false;
      poppedUp = 1;
    }
  }

  void MoveServoDown() {
    if (currentPosition > minPosition) { //if there was more movement left in this period
      currentPosition -= dPosition; //set new position to move to
      if (currentPosition < minPosition) //don't allow less than the minimum
        currentPosition = minPosition;
      SetPosition(currentPosition); 
    } else {
      moving = false;
      poppedUp = 0;
    }
  }
};

Popup p1 = Popup(0); //slot 1
Popup p2 = Popup(1); //slot 2

const int buttonPin = 2;  //D2
bool buttonPressed = false;
bool buttonHeld = false;
unsigned long pressStartTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Popup logic");

  Wire.begin();       // Start I2C communication
  pwm.begin();        // Start the servo controller
  pwm.setPWMFreq(50); // Set frequency to 50Hz for servos

  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor

  p1.FoceDown();
  p2.FoceDown();
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin) == LOW;

  if (currentButtonState && !buttonPressed) { //button press detected
    buttonPressed = true;
    pressStartTime = millis();

    p1.RegisterButtonPress();
    p2.RegisterButtonPress();

  } else if (currentButtonState && buttonPressed) { //button hold detected
    long pressDuration = millis() - pressStartTime;
    if (pressDuration > 400) {

      if (p1.poppedUp == p2.poppedUp) { //popups are in sync

        //gives p1 one more press to accomodate for the extra movement
        if (p1.unhandledPresses == p2.unhandledPresses)
          p1.RegisterButtonPress(); 

        //only tick p1 while popups are synced
        p1.Tick(); 
        delay(Popup::delayMs);
        return;

      } else { //popups are out of sync, continue normal logic
        p1.RegisterButtonPress();
        p2.RegisterButtonPress();
      }
    }
  }

  if (!currentButtonState && buttonPressed) {
    buttonPressed = false;
  }

  p1.Tick();
  p2.Tick();
  delay(Popup::delayMs);

  //button is not pressed and p1 has finished moving and p1 has no more button presses and popups are out of sync
  if (!currentButtonState && !p1.moving && !p1.unhandledPresses && p1.poppedUp != p2.poppedUp) {
    p1.poppedUp ? p2.ForceUp() : p2.FoceDown(); //reset the flipped popup
    p2.unhandledPresses = 0;
  }
}
