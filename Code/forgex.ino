/**
 * 4WD Robot Control with ESP32, L298N, FlySky i6X, 16x2 I2C LCD
 * + 4 Servo Robotic Arm Added
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// -------------------- PIN DEFINITIONS --------------------

// Left Side L298N (Driver #1)
const int LF_PWM_PIN = 32;
const int LB_PWM_PIN = 18;
const int LF_IN1 = 33;      
const int LF_IN2 = 25;
const int LB_IN3 = 23;      
const int LB_IN4 = 5;       

// Right Side L298N (Driver #2)
const int RF_PWM_PIN = 26;
const int RB_PWM_PIN = 19;
const int RF_IN1 = 27;      
const int RF_IN2 = 14;
const int RB_IN3 = 12;      
const int RB_IN4 = 13;      

// FlySky Receiver PWM Input Pins (Drive)
const int CH1_PIN = 34; 
const int CH2_PIN = 35; 

// -------------------- ARM SERVO SECTION --------------------

// Servo Output Pins
const int SERVO_BASE_ROT = 4;      
const int SERVO_BASE_TILT = 15;    
const int SERVO_ELBOW = 16;        
const int SERVO_GRIPPER = 17;      

// FlySky Receiver PWM Input Pins (Arm)
const int CH3_PIN = 36;  
const int CH4_PIN = 39;  
const int CH5_PIN = 2;   
const int CH6_PIN = 0;   

// Servo PWM Settings
const int SERVO_FREQ = 50;
const int SERVO_RES = 16;

// -------------------- LCD SETTINGS --------------------

LiquidCrystal_I2C lcd(0x27, 16, 2); 

// -------------------- MOTOR PWM SETTINGS --------------------

const int PWM_FREQ = 5000;
const int PWM_RES = 8; 

// -------------------- CONTROL VARIABLES --------------------

const int DEADZONE = 40;
unsigned long lastUpdate = 0;
String currentAction = "STOPPED";

// ============================================================
// ========================== SETUP ============================
// ============================================================

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("FORGEX IS READY");
  delay(1500);
  lcd.clear();

  pinMode(LF_IN1, OUTPUT); pinMode(LF_IN2, OUTPUT);
  pinMode(LB_IN3, OUTPUT); pinMode(LB_IN4, OUTPUT);
  pinMode(RF_IN1, OUTPUT); pinMode(RF_IN2, OUTPUT);
  pinMode(RB_IN3, OUTPUT); pinMode(RB_IN4, OUTPUT);

  // Motor PWM
  ledcAttach(LF_PWM_PIN, PWM_FREQ, PWM_RES);
  ledcAttach(LB_PWM_PIN, PWM_FREQ, PWM_RES);
  ledcAttach(RF_PWM_PIN, PWM_FREQ, PWM_RES);
  ledcAttach(RB_PWM_PIN, PWM_FREQ, PWM_RES);

  // Receiver Inputs
  pinMode(CH1_PIN, INPUT);
  pinMode(CH2_PIN, INPUT);
  pinMode(CH3_PIN, INPUT);
  pinMode(CH4_PIN, INPUT);
  pinMode(CH5_PIN, INPUT);
  pinMode(CH6_PIN, INPUT);

  // Servo PWM
  ledcAttach(SERVO_BASE_ROT, SERVO_FREQ, SERVO_RES);
  ledcAttach(SERVO_BASE_TILT, SERVO_FREQ, SERVO_RES);
  ledcAttach(SERVO_ELBOW, SERVO_FREQ, SERVO_RES);
  ledcAttach(SERVO_GRIPPER, SERVO_FREQ, SERVO_RES);
}

// ============================================================
// ============================ LOOP ==========================
// ============================================================

void loop() {

  // ---------------- DRIVE CONTROL ----------------

  int ch1_raw = pulseIn(CH1_PIN, HIGH, 25000); 
  int ch2_raw = pulseIn(CH2_PIN, HIGH, 25000); 

  int steer = map(ch1_raw, 1000, 2000, -255, 255);
  int drive = map(ch2_raw, 1000, 2000, -255, 255);

  if (abs(drive) < DEADZONE && abs(steer) < DEADZONE) {
    currentAction = "STOPPED   ";
    steer = 0; drive = 0;
  } 
  else if (drive > DEADZONE) {
    if (steer > DEADZONE)       currentAction = "TURN RIGHT";
    else if (steer < -DEADZONE) currentAction = "TURN LEFT ";
    else                        currentAction = "FORWARD   ";
  } 
  else if (drive < -DEADZONE) {
    if (steer > DEADZONE)       currentAction = "REV-RIGHT ";
    else if (steer < -DEADZONE) currentAction = "REV-LEFT  ";
    else                        currentAction = "REVERSE   ";
  } 
  else if (abs(steer) > DEADZONE) {
    currentAction = (steer > 0) ? "SPIN RIGHT" : "SPIN LEFT ";
  }

  int leftSpeed = drive + steer;
  int rightSpeed = drive - steer;

  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  controlMotors(leftSpeed, rightSpeed);

  // ---------------- ARM CONTROL ----------------

  int ch3 = pulseIn(CH3_PIN, HIGH, 25000);
  int ch4 = pulseIn(CH4_PIN, HIGH, 25000);
  int ch5 = pulseIn(CH5_PIN, HIGH, 25000);
  int ch6 = pulseIn(CH6_PIN, HIGH, 25000);

  int baseRotAngle  = map(ch3, 1000, 2000, 0, 180);
  int baseTiltAngle = map(ch4, 1000, 2000, 30, 150);
  int elbowAngle    = map(ch5, 1000, 2000, 20, 160);
  int gripAngle     = map(ch6, 1000, 2000, 40, 120);

  setServoAngle(SERVO_BASE_ROT, baseRotAngle);
  setServoAngle(SERVO_BASE_TILT, baseTiltAngle);
  setServoAngle(SERVO_ELBOW, elbowAngle);
  setServoAngle(SERVO_GRIPPER, gripAngle);

  if (millis() - lastUpdate > 200) {
    updateLCD(leftSpeed, rightSpeed);
    lastUpdate = millis();
  }
}

// ============================================================
// ======================= FUNCTIONS ==========================
// ============================================================

void setServoAngle(int pin, int angle) {
  angle = constrain(angle, 0, 180);

  int minDuty = 1638;   
  int maxDuty = 8192;   

  int duty = map(angle, 0, 180, minDuty, maxDuty);
  ledcWrite(pin, duty);
}

void updateLCD(int l, int r) {
  lcd.setCursor(0, 0);
  lcd.print("ACT: ");
  lcd.print(currentAction);
  
  lcd.setCursor(0, 1);
  lcd.print("L:");
  lcd.print(l);
  lcd.print("   R:");
  lcd.print(r);
  lcd.print("    ");
}

void controlMotors(int left, int right) {

  if (left > 0) {
    digitalWrite(LF_IN1, HIGH); digitalWrite(LF_IN2, LOW);
    digitalWrite(LB_IN3, HIGH); digitalWrite(LB_IN4, LOW);
    ledcWrite(LF_PWM_PIN, left);
    ledcWrite(LB_PWM_PIN, left);
  } else if (left < 0) {
    digitalWrite(LF_IN1, LOW);  digitalWrite(LF_IN2, HIGH);
    digitalWrite(LB_IN3, LOW);  digitalWrite(LB_IN4, HIGH);
    ledcWrite(LF_PWM_PIN, abs(left));
    ledcWrite(LB_PWM_PIN, abs(left));
  } else {
    digitalWrite(LF_IN1, LOW);  digitalWrite(LF_IN2, LOW);
    digitalWrite(LB_IN3, LOW);  digitalWrite(LB_IN4, LOW);
    ledcWrite(LF_PWM_PIN, 0);
    ledcWrite(LB_PWM_PIN, 0);
  }

  if (right > 0) {
    digitalWrite(RF_IN1, HIGH); digitalWrite(RF_IN2, LOW);
    digitalWrite(RB_IN3, HIGH); digitalWrite(RB_IN4, LOW);
    ledcWrite(RF_PWM_PIN, right);
    ledcWrite(RB_PWM_PIN, right);
  } else if (right < 0) {
    digitalWrite(RF_IN1, LOW);  digitalWrite(RF_IN2, HIGH);
    digitalWrite(RB_IN3, LOW);  digitalWrite(RB_IN4, HIGH);
    ledcWrite(RF_PWM_PIN, abs(right));
    ledcWrite(RB_PWM_PIN, abs(right));
  } else {
    digitalWrite(RF_IN1, LOW);  digitalWrite(RF_IN2, LOW);
    digitalWrite(RB_IN3, LOW);  digitalWrite(RB_IN4, LOW);
    ledcWrite(RF_PWM_PIN, 0);
    ledcWrite(RB_PWM_PIN, 0);
  }
}


