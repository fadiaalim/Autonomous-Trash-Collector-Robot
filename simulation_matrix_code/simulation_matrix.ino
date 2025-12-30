#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Servo channels
#define BASE_CH      0
#define SHOULDER_CH  1
#define ELBOW_CH     2
#define GRIPPER_CH   3

// Servo pulse limits (هتظبطهم حسب السيرفو)
#define SERVO_MIN  110   // ~500us
#define SERVO_MAX  520   // ~2500us

int step = 10; // حركة صغيرة = سمووث

// Rest position
int basePulse     = 320;
int shoulderPulse = 320;
int elbowPulse    = 320;
int gripperPulse  = 320;

void setup() {
  Serial.begin(9600);

  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(50);   // ⭐ frequency الصح للسيرفو

  delay(10);

  moveAll();

  Serial.println("PCA9685 Keyboard Control Ready");
  Serial.println("q/a Base | w/s Shoulder | e/d Elbow | r/f Gripper");
}

void loop() {
  if (Serial.available()) {
    char key = Serial.read();

    switch (key) {
      case 'q': basePulse += step; break;
      case 'a': basePulse -= step; break;

      case 'w': shoulderPulse += step; break;
      case 's': shoulderPulse -= step; break;

      case 'e': elbowPulse += step; break;
      case 'd': elbowPulse -= step; break;

      case 'r': gripperPulse += step; break;
      case 'f': gripperPulse -= step; break;
    }

    basePulse     = constrain(basePulse, SERVO_MIN, SERVO_MAX);
    shoulderPulse = constrain(shoulderPulse, SERVO_MIN, SERVO_MAX);
    elbowPulse    = constrain(elbowPulse, SERVO_MIN, SERVO_MAX);
    gripperPulse  = constrain(gripperPulse, SERVO_MIN, SERVO_MAX);

    moveAll();

    // Matrix output
    Serial.print("[ ");
    Serial.print(basePulse); Serial.print(" , ");
    Serial.print(shoulderPulse); Serial.print(" , ");
    Serial.print(elbowPulse); Serial.print(" , ");
    Serial.print(gripperPulse);
    Serial.println(" ]");

    delay(20); // يقلل jitter
  }
}

void moveAll() {
  pwm.setPWM(BASE_CH, 0, basePulse);
  pwm.setPWM(SHOULDER_CH, 0, shoulderPulse);
  pwm.setPWM(ELBOW_CH, 0, elbowPulse);
  pwm.setPWM(GRIPPER_CH, 0, gripperPulse);
}
