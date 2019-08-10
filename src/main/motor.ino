#include "cansat_define.h"

// Initialize function for motor pin
void motor_init(){
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(pwma, OUTPUT);

  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);
  pinMode(pwmb, OUTPUT);

  digitalWrite(ain1, LOW);
  digitalWrite(ain2, LOW);
  digitalWrite(pwma, LOW);

  digitalWrite(bin1, LOW);
  digitalWrite(bin2, LOW);
  digitalWrite(pwmb, LOW);
}

void left_cw(int pwm){
  if(pwm > 255) pwm = 255;
  if(pwm < 0) pwm = 0;

  digitalWrite(ain1, LOW);
  digitalWrite(ain2, HIGH);
  analogWrite(pwma, pwm);
}

void left_ccw(int pwm){
  if(pwm > 255) pwm = 255;
  if(pwm < 0) pwm = 0;

  digitalWrite(ain2, LOW);
  digitalWrite(ain1, HIGH);
  analogWrite(pwma, pwm);
}

void right_cw(int pwm){
  if(pwm > 255) pwm = 255;
  if(pwm < 0) pwm = 0;

  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);
  analogWrite(pwmb, pwm);
}

void right_ccw(int pwm){
  if(pwm > 255) pwm = 255;
  if(pwm < 0) pwm = 0;

  digitalWrite(bin2, LOW);
  digitalWrite(bin1, HIGH);
  analogWrite(pwmb, pwm);
}

void forward(int pwm){
  if(pwm > 255) pwm = 255;
  if(pwm < 0) pwm = 0;

  right_cw(pwm);
  left_ccw(pwm);
}

void back(int pwm){
  if(pwm > 255) pwm = 255;
  if(pwm < 0) pwm = 0;

  right_ccw(pwm);
  left_cw(pwm);
}

void turn_right(int pwm){
  if(pwm > 255) pwm = 255;
  if(pwm < 0) pwm = 0;

  right_ccw(pwm);
  left_ccw(pwm);
}

void turn_left(int pwm){
  if(pwm > 255) pwm = 255;
  if(pwm < 0) pwm = 0;
  
  right_cw(pwm);
  left_cw(pwm);
}

void motor_stop(){
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, LOW);
  digitalWrite(pwma, LOW);

  digitalWrite(bin1, LOW);
  digitalWrite(bin2, LOW);
  digitalWrite(pwmb, LOW);
}

void rollover_return(){
  right_cw(255);
  delay(2000);
  right_ccw(255);
  delay(1000);
  motor_stop();
  delay(500);
  right_ccw(255);
  delay(2000);
  right_cw(255);
  delay(1000);
  motor_stop();
  delay(500);
}
