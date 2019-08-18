/*
 * ARLISS2019 CanSat
 * 
 * Author: ymt
 * Sensor: LSM6, LPS, LIS3MDL, GPS, CdS
 */

// Read CanSat definition file
#include "cansat_define.h"
#include "cansat_prototype.h"

// Intterupt function called when the button is pressed.
void _sw_pushed(){
  Serial.println("USR Pushed!!!");
  press_counter++;
  beep(PUSHED);
  if(press_counter > 2){
    s = State_exit;
  }
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
  writeFile("/hello.txt", "Hello CanSat!!!\n");
  writeFile("/log.csv",
            "millis,year,month,day,hour,minute,second,lat,lng,ax,ay,az,gx,gy,gz,mx,my,mz,pressure,altitude,temperature,cds,batt1,batt2\n");
  
  // If the previous exit status is non-zero, the previous status is assigned to variable s
  int old_status = readStatus("status.txt");
  Serial.print("old_status: "); Serial.println(old_status);
  if(old_status == (int)State_test)  s = State_test;
  else if(old_status == (int)State_test2) s = State_test2;
  else if(old_status == (int)State_exit)  s = State_exit;
  else {                                  s = State_test; }
  writeStatus();

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
  //timer_init_3();
  //timer_init_4();

  delay(20);
}


/***************************************************************
 * MAIN LOOP
 **************************************************************/
void loop() {

  switch(s){
    
    case State_test:
      // test code
      writeStatus();

      imu_plot();
      break;

    case State_test2:
      writeStatus();

      Serial.println("State_test2");
      break;

    case State_exit:
      writeStatus();

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
      writeStatus();
      Serial.println("default");
      break;
  }
}
