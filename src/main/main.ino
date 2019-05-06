/*
 * ARLISS2019 CanSat
 * 
 * Author: ymt
 * Secsor: LSM6, LPS, LIS3MDL, GPS
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

  beep(BOOT_UP);
  delay(100);
}

void loop() {
  imu_read();
  gps_read();
  pwm_test();
}
