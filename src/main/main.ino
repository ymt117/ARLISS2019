/***************************************************************
 * *************************************************************
 * ARLISS2019 CanSat Competition 350ml class
 * Team: 100kinSAT
 * Author: ymt
 * Sensor: LSM6, LPS, LIS3MDL, GPS, CdS
 * *************************************************************
 ***************************************************************/

// Read CanSat definition file
#include "cansat_define.h"
#include "cansat_prototype.h"

// Intterupt function called when the button is pressed.
void _sw_pushed(){

  press_counter++;
  Serial.print("press_counter: "); Serial.println(press_counter);
  beep(PUSHED);
}

/***************************************************************
 * 
 * SETUP
 * Called only once
 * 
 **************************************************************/
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("[State: Initialization] ... Start CanSat Program!!!");

  // Initialize the microSD Card
  sd_init();
  writeFile("/hello.txt", "Hello CanSat!!!\n");
  writeFile("/log.csv",
            "millis,state,year,month,day,hour,minute,second,lat,lng,ax,ay,az,gx,gy,gz,mx,my,mz,pressure,altitude,temperature,cds,batt1,batt2\n");
  
  // If the previous exit status is non-init or non-exit, the previous status is assigned to variable s.
  /*
  int old_status = readStatus("status.txt");
  Serial.print("old_status: "); Serial.println(old_status);
  if(old_status == (int)State_launch_detect) s = State_launch_detect;
  else if(old_status == (int)State_release_detect) s = State_release_detect;
  else if(old_status == (int)State_drop_detect) s = State_drop_detect;
  else if(old_status == (int)State_test)  s = State_test;
  else if(old_status == (int)State_exit)  s = State_exit;
  else {                                  s = State_test; }
  */
  s = State_run_to_goal;
  writeStatus();

  delay(20);

  // Initialize the I/O
  led_init();
  speaker_init();
  motor_init();
  heat_init();
  pinMode(cds, INPUT);

  delay(20);

  // Initialize Push Switch
  pinMode(sw, INPUT);
  attachInterrupt(sw, _sw_pushed, RISING);

  // Initialize Monitor Battery voltage
  pinMode(batt1, INPUT_PULLUP);
  pinMode(batt2, INPUT_PULLUP);

  // Initialize TWE-Lite Sleep Pin
  pinMode(twe_lite_sleep_pin, OUTPUT);
  twe_lite_wakeup();

  // Initialize the sensor
  imu_init();
  gps_init();

  delay(20);

  // Sensor offset
  imu_offset();
  altitude_offset();

  madgwick_init();

  beep(BOOT_UP);

  // Timer init
  //timer_start_3();
  //timer_start_4();

  timer=millis();
  delay(20);
  counter=0;

  microsPerReading = 1000000 / 50;
  microsPrevious = micros();
}


/***************************************************************
 * 
 * MAIN LOOP
 * 
 **************************************************************/
void loop() {

  switch(s){

    case State_launch_detect:
      Serial.println("[State: Launch detection]");
      writeStatus();
      launch_detect();
      break;

    case State_release_detect:
      Serial.println("[State: Release detection]");
      writeStatus();
      release_detect();
      break;

    case State_drop_detect:
      Serial.println("[State: Drop detection]");
      writeStatus();
      drop_detect();
      break;

    case State_first_fire:
      Serial.println("[State: First fire]");
      writeStatus();
      delay(20000);
      _heat1();
      _heat1();
      s = State_second_fire;
      break;

    case State_second_fire:
      Serial.println("[State: Second fire]");
      writeStatus();
      delay(20000);
      _heat2();
      _heat2();
      forward(255);
      delay(500);
      motor_stop();
      delay(500);
      s = State_run_to_goal;
      break;

    case State_run_to_goal:
      Serial.println("[State: Run to GOAL]");
      writeStatus();
      //run2goal_mag();
      run2goal();
      break;

    case State_goal:
      writeStatus();
      Serial.println("[State: GOAL] ... All done!!!");
      beep(CLANNAD);
      delay(1000);
      while(1) blink();
      break;
    
    case State_test:
      // test code
      writeStatus();
      Serial.println("[State: Test mode]");
      heading_test();
      break;

    case State_exit:
      writeStatus();
      // End processing and enter infinite loop...
      Serial.println("[State: EXIT] ... Gooodbye~~");
      led_off();
      motor_stop();
      beep(SHUT_DOWN);
      delay(1000);
      while(1);
      break;

    default:
      // code block
      writeStatus();
      Serial.println("State: default");
      break;
  }
}
