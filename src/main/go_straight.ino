//====================================================
//直進　（ジャイロ）　→　（偏角）　→　（PWM）
//
//Gyro:2000dps ODR:1.66[khz] (enabledDefault)
//
//loop_num (1/ about 30（+α）ms)
//====================================================
#include "cansat_define.h"
#include "cansat_prototype.h"

float global_theta_z = 0.0;

void go_straight(int end_time, int diff_direction){
  unsigned long start_time = millis();
  float gyro_z_array[3];
  float theta = 0;
  float theta_z = 0;

  //Serial.print("Moving forward "); Serial.print(end_time); Serial.println(" ms");
  forward(255);

  while(millis() - start_time < end_time){

    for(int i=0; i<3; i++){
      Read_Gyro();
      gyro_z_array[i] = gyro_z * 0.07;

      if(i < 2) delay(5);
    }

    theta += (gyro_z_array[0] + 4 * gyro_z_array[1] + gyro_z_array[3]) / 600 * 1.5;

    if(theta > 180) theta = -180;
    if(theta < -180) theta = 180;

    theta_z = theta + diff_direction + global_theta_z;

    // Motor strength: VeryHigh[255] High[200] Medium[175] Low[150] VeryLow[100]
    if(theta_z > 45){
      // right: VeryLow, left: VeryHigh
      right_cw(100); left_ccw(255);
    }
    else if(theta_z <= 45 && theta_z > 20){
      // right: Medium, left: VeryHigh
      right_cw(150); left_ccw(255);
    }
    else if(theta_z <= 20 && theta_z > 10){
      // right: Medium, left: VeryHigh
      right_cw(175); left_ccw(255);
    }
    else if(theta_z <= 10 && theta_z > 2){
      // right: High, left: VeryHigh
      right_cw(200); left_ccw(255);
    }

    else if(theta_z < -45){
      // right: VeryHigh, left: VeryLow
      right_cw(255); left_ccw(100);
    }
    else if(theta_z >= -45 && theta_z < -20){
      // right: VeryHigh, left: Low
      right_cw(255); left_ccw(150);
    }
    else if(theta_z >= -20 && theta_z < -10){
      // right: VeryHigh, left: Medium
      right_cw(255); left_ccw(175);
    }
    else if(theta_z >= -10 && theta_z < -2){
      // right: VeryHigh, left: High
      right_cw(255); left_ccw(200);
    }

    else{
      // do nothing
    }
  }

  motor_stop();
  
  for(int i=0; i<3; i++){
    Read_Gyro();
    gyro_z_array[i] = gyro_z * 0.07;

    if(i < 2) delay(5);
  }

  theta += (gyro_z_array[0] + 4 * gyro_z_array[1] + gyro_z_array[3]) / 600 * 1.5;

  if(theta > 180) theta = -180;
  if(theta < -180) theta = 180;

  global_theta_z = theta + diff_direction + global_theta_z;

  delay(500);
  //beep(PUSHED);
}
