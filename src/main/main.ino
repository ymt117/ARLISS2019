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

#define LAUNCH_MODE 0 // 0のときモードA, 1のときモードB

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
  //delay(1000);
  Serial.begin(115200);
  Serial.println("[State: Initialization] ... Start CanSat Program!!!");

  // Initialize the microSD Card
  sd_init();
  writeFile("/hello.txt", "Hello CanSat!!!\n");
  writeFile("/log.csv",
            "millis,state,year,month,day,hour,minute,second,lat,lng,ax,ay,az,gx,gy,gz,mx,my,mz,pressure,altitude,temperature,cds,batt1,batt2,distance2goal\n");
  writeFile("/system_log.txt", "******************** start program ********************\n");
  writeFile("/system_log.txt", "[State: Initialization] ... Start CanSat Program!!!\n");
  writeFile("/control.csv", "lat,lng,control\n");
  
  // If the previous exit status is non-init or non-exit, the previous status is assigned to variable s.
  int old_status = readStatus();
  if(old_status <= 0){
    Serial.println("Reading status failed. Start program from state: State_launch_detect");
    s = State_launch_detect;
  }
  else{
    Serial.print("Start program from previous state: "); Serial.println(old_status);
    s = (MyState)old_status;
  }
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

  beep(BOOT_UP);

  // Timer init
  //timer_start_3();
  //timer_start_4();
  //timer_start_12();

  timer=millis();
  delay(20);
  counter=0;

  madgwick_init();
  microsPerReading = 1000000 / 50;
  microsPrevious = micros();

  // 打ち上げ前(ロケット搭載時)にスリープに入る
  
  if(s == State_launch_detect){
    Serial.print("Sleep ... ");
    writeFile("/system_log.txt", "Sleep ... ");
    twe_lite_sleep();
    low_power();
    twe_lite_wakeup();
    Serial.println("Wake up!!!");
    writeFile("/system_log.txt", "Wake up!!!\n");
  }
  

  beep(BOOT_UP);
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
      writeFile("/system_log.txt", "\n[State: Launch detection]\n");
      writeStatus();
      writeAll();
      // Turn off the radio in launch detection mode
      // 打上検知モードでは無線機をオフにする
      //twe_lite_sleep();
      if(0 == LAUNCH_MODE){// 0
        launch_detect_A();
      }
      else{// 1
        launch_detect_B();
      }
      break;

    case State_release_detect:
      Serial.println("[State: Release detection]");
      writeFile("/system_log.txt", "\n[State: Release detection]\n");
      writeStatus();
      writeAll();
      release_detect();
      break;

    case State_drop_detect:
      Serial.println("[State: Drop detection]");
      writeFile("/system_log.txt", "\n[State: Drop detection]\n");
      writeStatus();
      writeAll();
      if(0 == LAUNCH_MODE){// 0
        drop_detect_A();
      }
      else{// 1
        drop_detect_B();
      }
      break;

    case State_first_fire:
      Serial.println("[State: First fire]");
      writeFile("/system_log.txt", "\n[State: First fire]\n");
      writeStatus();
      writeAll();
      delay(500);
      _heat1();
      _heat1();
      s = State_second_fire;
      break;

    case State_second_fire:
      Serial.println("[State: Second fire]");
      writeFile("/system_log.txt", "\n[State: Second fire]\n");
      writeStatus();
      writeAll();
      if(0 == LAUNCH_MODE) delay(500);
      _heat2();
      _heat2();
      forward(255);
      delay(10000);
      motor_stop();
      delay(1000);
      s = State_run_to_goal;
      break;

    case State_run_to_goal:
      Serial.println("[State: Run to GOAL]");
      writeFile("/system_log.txt", "\n[State: Run to GOAL]\n");
      writeStatus();
      writeAll();
      // Turn on the radio
      // 無線機をオンにする
      twe_lite_wakeup();
      run2goal();
      break;

    case State_goal:
      Serial.println("[State: GOAL] ... All done!!!");
      writeFile("/system_log.txt", "\n[State: GOAL] ... All done!!!\n");
      writeStatus();
      writeAll();
      motor_stop();
      digitalWrite(led2, HIGH);
      delay(2000);
      while(1){
        beep(CLANNAD);
        delay(1000);
      }
      //while(1) blink();
      break;
    
    case State_test:
      // test code
      Serial.println("[State: Test mode]");
      writeFile("/system_log.txt", "\n[State: Test mode]\n");
      writeStatus();
      writeAll();
      delay(1000);
      beep(PUSHED);
      _heat1();
      delay(1000);
      beep(PUSHED);
      beep(PUSHED);
      _heat2();
      sw_motor();
      break;

    case State_exit:
      // End processing and enter infinite loop...
      Serial.println("[State: EXIT] ... Gooodbye~~");
      writeFile("/system_log.txt", "\n[State: EXIT] ... Gooodbye~~\n");
      writeStatus();
      led_off();
      motor_stop();
      beep(SHUT_DOWN);
      delay(1000);
      while(1);
      break;

    default:
      // code block
      Serial.println("[State: default]");
      writeFile("/system_log.txt", "\n[State: default]\n");
      writeStatus();
      break;
  }
}
