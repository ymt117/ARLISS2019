/*
 * ARLISS2019 CanSat
 * 
 * Author: ymt
 * Sensor: LSM6, LPS, LIS3MDL, GPS
 */

#include "cansat_define.h"

// State transition of CanSat
enum MyState{
  State_test = 0,
  State_exit
};
MyState s = State_test;

void _exit(){
  s = State_exit;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Start CanSat Program!!!");

  // Initialize the sensor
  //imu_init();
  //gps_init();
  //sd_init();
  //writeFile("/hello.txt", "Hello CanSat!!!");

  //pwm_init();
  led_init();
  pinMode(cds, INPUT);

  // Set button
  //pinMode(PC13, INPUT_PULLUP);
  attachInterrupt(PC13, _exit, RISING);

  beep(BOOT_UP);
  delay(100);
}

void loop() {
  switch(s){
    
    case State_test:
      //imu_read();
      //gps_read();
      //pwm_test();
      Serial.println(analogRead(cds));
      delay(100);
      break;
    case State_exit:
      Serial.println("Goodbye~~");
      beep(SHUT_DOWN);
      exit(0);
      break;
    default:
      // code block
      break;
  }
}
