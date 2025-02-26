const int buttonPin = 2;  //D2
bool buttonPressed = false;
unsigned long pressStartTime = 0;

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor
}

void loop() {
  bool currentButtonState = digitalRead(buttonPin) == LOW;  // Button is active LOW

  //button press detected
  if (currentButtonState && !buttonPressed) {
    buttonPressed = true;
    pressStartTime = millis();
    Serial.println("Button press detected");
  
  //button hold detected
  } else if (currentButtonState && buttonPressed) {
    long pressDuration = millis() - pressStartTime;
    
    if (pressDuration > 400) {
      Serial.print("Button held for: ");
      Serial.print(pressDuration);
      Serial.println(" ms");
    }
  }

  //button lifted
  if (!currentButtonState && buttonPressed) {
    buttonPressed = false;
    Serial.println("Button let go");
    delay(20);
  }
}
