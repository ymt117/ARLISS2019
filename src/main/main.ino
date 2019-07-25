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
  writeFile("/hello.txt", "Hello CanSat!!!");
  writeFile("/acc_g.csv", "millis,raw_ax,raw_ay,raw_az,ax_g,ay_g,az_g,a\n");
  writeFile("/mag.csv", "mx_raw,my_raw,mz_raw,mx,my,mz\n");
  writeFile("/mag_offset.csv", "mx_max,mx_min,my_max,my_min,mz_max,mz_min\n");

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

  //compass_calibrate();

  timer=millis();
  delay(20);
  counter=0;

  // Timer init
  timer_init();

  madgwick_init();

  beep(BOOT_UP);
  delay(20);
}


/***************************************************************
 * MAIN LOOP
 **************************************************************/
void loop() {
  float lng1 = 130.543952;
  float lat1 = 31.568171;
  float lng2 = 130.542494;
  float lat2 = 31.568353;

  switch(s){
    
    case State_test:
      Serial.print(distance(lng1, lat1, lng2, lat2));
      Serial.print('\t');
      Serial.println(direction(lng1, lat1, lng2, lat2));

      Serial.print(distance(130.543426, 31.567485, 130.543343, 31.568532));
      Serial.print('\t');
      Serial.println(direction(130.543426, 31.567485, 130.543343, 31.568532));
      //madgwick_update();
      //imu_test();
      //mag_calibrate();
      //test_run();
      //static_load();
      //imu_plot();
      //gps_read();
      //Serial.println(analogRead(cds));
      //delay(100);
      //blink();
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
