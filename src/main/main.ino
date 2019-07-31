/*
 * ARLISS2019 CanSat
 * 
 * Author: ymt
 * Sensor: LSM6, LPS, LIS3MDL, GPS, CdS
 */

// Read CanSat definition file
#include "cansat_define.h"
#include "cansat_prototype.h"

// State transition of CanSat
enum MyState{
  State_test = 0,
  State_exit
};
MyState s = State_test;

// Intterupt function called when the button is pressed.
void _sw_pushed(){
  press_counter++;
}

/***************************************************************
 * SETUP
 **************************************************************/
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("Start CanSat Program!!!");

  // Initialize the microSD Card
  sd_init();
  writeFile("/imu.csv", "millis,ax,ay,az,gx,gy,gz,mx,my,mz\n");
  writeFile("/lps.csv", "millis,pressure,altitude,temperature\n");
  writeFile("/gps.csv", "millis,lat,lng\n");

  delay(20);

  // Initialize the I/O
  led_init();
  speaker_init();
  motor_init();
  pinMode(cds, INPUT);

  delay(20);

  // Initialize Push Switch
  pinMode(sw, INPUT_PULLUP);
  attachInterrupt(sw, _sw_pushed, RISING);

  // Initialize Monitor Battery voltage
  pinMode(batt1, INPUT_PULLUP);
  pinMode(batt2, INPUT_PULLUP);

  // Initialize Heat Pin
  pinMode(heat1, OUTPUT);
  digitalWrite(heat1, LOW);
  pinMode(heat2, OUTPUT);
  digitalWrite(heat2, LOW);

  // Initialize TWE-Lite Sleep Pin
  pinMode(twe_lite_sleep, OUTPUT);
  digitalWrite(twe_lite_sleep, HIGH);

  // Initialize the sensor
  imu_init();
  gps_init();

  delay(20);

  imu_offset();

  timer=millis();
  delay(20);
  counter=0;

  madgwick_init();

  beep(BOOT_UP);

  // Timer init
  timer_init();
  delay(20);
}


/***************************************************************
 * MAIN LOOP
 **************************************************************/
void loop() {

  switch(s){
    
    case State_test:
      // test code
      //dd_test();
      //heat_test2();
      break;
    case State_exit:
      // End processing and enter infinite loop...
      Serial.println("Goodbye~~");
      led_off();
      motor_stop();
      beep(SHUT_DOWN);
      delay(1000);
      while(1);
      break;
    default:
      // code block
      break;
  }
}
