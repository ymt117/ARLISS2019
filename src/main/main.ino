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
  switch_counter++;
  if(switch_counter >= 2)
    s = State_exit;
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("Start CanSat Program!!!");

  // Initialize the microSD Card
  sd_init();
  writeFile("/hello.txt", "Hello CanSat!!!");
  writeFile("acc_g.csv", "millis,raw_ax,raw_ay,raw_az,ax_g,ay_g,az_g,a\n");

  led_init();
  speaker_init();
  pinMode(cds, INPUT);

  // Set button
  pinMode(sw, INPUT_PULLUP);
  attachInterrupt(sw, _exit, RISING);

  // Initialize the sensor
  imu_init();
  gps_init();

  beep(BOOT_UP);
  delay(100);
}

void loop() {
  switch(s){
    
    case State_test:
      static_load();
      //imu_plot();
      //gps_read();
      //Serial.println(analogRead(cds));
      //delay(100);
      //blink();
      break;
    case State_exit:
      Serial.println("Goodbye~~");
      led_off();
      beep(SHUT_DOWN);
      exit(0);
      break;
    default:
      // code block
      break;
  }
}
