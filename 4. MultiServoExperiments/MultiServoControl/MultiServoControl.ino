#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40); ;
int servoCount = 1;
int minAngle = 100;
int maxAngle = 600;
int servoDelay = 1000;

void setup() {
  Serial.begin(9600);
  Serial.println("Multi servo test");

  pwm.begin();

  Wire.begin();       // Start I2C communication
  pwm.begin();        // Start the servo controller
  pwm.setPWMFreq(50); // Set frequency to 50Hz for servos

  Serial.println("Servo controller ready!");
}

int reps = 0;
int repMax = 3;

void loop() {
  if (reps < repMax) {
    Oscillate(0);
    Oscillate(1);

    Serial.print("Rep: ");
    Serial.println(reps);
    reps++;
  } else {
  }
}

void CalculateMinAngle() {
  for (int i = maxAngle; i >= minAngle; i -= 50) {
    Serial.print("Writing angle: ");
    Serial.println(i);
    pwm.setPWM(0, 0, i);
    delay(servoDelay);
  }
}

void Oscillate(int servo) {
  for (int i = minAngle; i <= maxAngle * 2 - minAngle; i+= 50) {

    int angleToWrite = i;

    if (i > maxAngle) 
      angleToWrite = maxAngle * 2 - i;

    Serial.print("Writing angle: ");
    Serial.println(angleToWrite);

    pwm.setPWM(servo, 0, angleToWrite);
    delay(30);
  }
}