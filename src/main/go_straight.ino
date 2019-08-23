//====================================================
//直進　（ジャイロ）　→　（偏角）　→　（PWM）
//
//Gyro:2000dps ODR:1.66[khz] (enabledDefault)
//
//loop_num (1/ about 30（+α）ms)
//====================================================
#include "cansat_define.h"
#include "cansat_prototype.h"

int thz = 0;

void go_straight(int m_second){
  
  unsigned int time_f = millis();
  unsigned int time_loop = 0; 
  float gz[3];
  float th = 0;
  int i, j = 0;
  
  thz = 0;
  
  forward(255);  //前進

  while((millis() - time_f) < m_second){
    
    time_loop = millis();
    for(i= 0; i< 3; i++){
      Read_Gyro();
      gz[i] = gyro_z * 2000;
      gz[i] = gz[i] / 32768;

      if(i < 2) delay(5);

    }
    Serial.print("ForLoop:"); Serial.println(millis() - time_loop);
    th += (gz[0] + 4 * gz[1] +gz[2]) / 600 * 1.5;        

    if(th > 180)  th = -180;
    if(th < -180)  th = 180;

    thz = th;
    Serial.print("thz[d]:"); Serial.println(thz);

    //強強:255　強:200　中:175　弱:150　弱弱:100 

    if (thz > 45) {
      //右　弱弱　左　強強
      right_cw(100);
      left_ccw(255);
    } else if (thz <= 45 && thz > 20) {
      //右　強強　左　弱
      right_cw(150);
      left_ccw(255);
    } else if (thz <= 20 && thz > 10) {
      //右　強強　左　中
      right_cw(175);
      left_ccw(255);
    } else if (thz <= 10 && thz > 2) {
      //右　強強　左　強
      right_cw(200);
      left_ccw(255);
    } 
      
    else if (thz < -45) {
      //右　強強　左　弱弱
      right_cw(255);
      left_ccw(100);
    } else if (thz >= -45 && thz < -20) {
      //右　強強　左　弱
      right_cw(255);
      left_ccw(150);
    } else if (thz >= -20 && thz < -10) {
      //右　強強　左　中
      right_cw(255);
      left_ccw(175);
    } else if (thz <= 10 && thz > 2) {
      //右　強強　左　強
      right_cw(255);
      left_ccw(200);
    } 
      
    else if (thz <= 2 && thz >= -2) {
      //右　強強　左　強強
      right_cw(255);
      left_ccw(255);
    }

    Serial.print("WhileLoop:"); Serial.println(millis() - time_loop);  
    j++;
  }

  motor_stop();//停止
  
}