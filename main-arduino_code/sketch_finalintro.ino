#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// --- بنسات المحركات والألتراسونيك ---
const int IN1 = 2; const int IN2 = 3;
const int IN3 = 4; const int IN4 = 5;
const int TRIG = 6; const int ECHO = 7;
int motorSpeed = 110; 

// --- قنوات الـ PCA9685 ---
#define BASE_CH 0
#define SHOULDER_CH 1
#define ELBOW_CH 2
// تم حذف GRIPPER_CH 3

// --- الوضعية الابتدائية (Matrix Start) ---
int startBase = 520;      
int startShoulder = 110;   
int startElbow = 230;     

// --- وضعية الهدف (Matrix Target) ---
int targetBase = 470;
int targetShoulder = 110;
int targetElbow = 260;

int curBase, curShoulder, curElbow;

// --- دالات التحكم ---
void setPulse(uint8_t channel, int pulse) {
  pulse = constrain(pulse, 150, 600); 
  pwm.setPWM(channel, 0, pulse);
}

void smoothPulse(uint8_t channel, int &fromPulse, int toPulse) {
  int step = (fromPulse < toPulse) ? 6 : -6; 
  while (abs(fromPulse - toPulse) > 6) {
    fromPulse += step;
    setPulse(channel, fromPulse);
    delay(15); 
  }
  fromPulse = toPulse;
  setPulse(channel, fromPulse);
}

void moveForward() {
  analogWrite(IN1, 0); analogWrite(IN2, motorSpeed);
  analogWrite(IN3, 0); analogWrite(IN4, motorSpeed);
}

void stopRobot() {
  analogWrite(IN1, 0); analogWrite(IN2, 0);
  analogWrite(IN3, 0); analogWrite(IN4, 0);
}

long getDistance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH) * 0.034 / 2;
}

// --- تنفيذ الحركة المعتمدة على الـ 3 محاور فقط ---
void pickAndPlace() {
  // 1. التحرك لنقطة الهدف (Matrix Target)
  smoothPulse(BASE_CH, curBase, targetBase); 
  smoothPulse(SHOULDER_CH, curShoulder, targetShoulder); 
  smoothPulse(ELBOW_CH, curElbow, targetElbow);
  delay(1000); // وقت انتظار عند الهدف

  // 2. الرفع والالتفاف للخلف (180 درجة للبوكس)
  smoothPulse(SHOULDER_CH, curShoulder, 250); // رفع للأمان
  smoothPulse(BASE_CH, curBase, 180); 
  delay(500);
  
  // 3. العودة لوضع البداية (Home)
  smoothPulse(ELBOW_CH, curElbow, startElbow);
  smoothPulse(SHOULDER_CH, curShoulder, startShoulder);
  smoothPulse(BASE_CH, curBase, startBase);
}

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT);

  // البداية بالقيم المطلوبة
  curBase = startBase; 
  curShoulder = startShoulder;
  curElbow = startElbow; 
  
  setPulse(BASE_CH, curBase);
  setPulse(SHOULDER_CH, curShoulder);
  setPulse(ELBOW_CH, curElbow);
  delay(1000);
}

void loop() {
  long distance = getDistance();
  
  if (distance > 20 || distance == 0) {
    moveForward();
  } else {
    stopRobot();
    Serial.println("DETECT"); 
    
    while (true) {
      if (Serial.available() > 0) {
        char cmd = Serial.read();
        if (cmd == 'P') {
          pickAndPlace();
          break; 
        } else if (cmd == 'S') {
          delay(1000); 
          break;
        }
      }
    }
  }
}