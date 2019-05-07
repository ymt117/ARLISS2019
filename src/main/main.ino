/*
 * ARLISS2019 CanSat
 * 
 * Author: ymt
 * Sensor: LSM6, LPS, LIS3MDL, GPS
 */

#include "spdef.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Start CanSat Program!!!");

  // Initialize the sensor
  imu_init();
  gps_init();
  sd_init();
  writeFile("/hello.txt", "Hello CanSat!!!");

  pwm_init();

  // Set button
  pinMode(PA4, INPUT_PULLUP);

  beep(BOOT_UP);
  delay(100);
}

void loop() {
  if(!digitalRead(PA4)){
    beep(SHUT_DOWN);
  }
  imu_read();
  gps_read();
  pwm_test();
}
