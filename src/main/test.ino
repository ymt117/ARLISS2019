#include "cansat_define.h"
#include "cansat_prototype.h"

// 静荷重試験用プログラム
void static_load(){
  char buf[1024];
  imu.read();
  unsigned long _millis = millis();

  float a = calc_g_force(imu.a.x, imu.a.y, imu.a.z);
  Serial.print(_millis); Serial.print("\t\t");
  Serial.print(a);

  if(a >= 10){
    Serial.print("\t\tOver 10G!!!");
  }

  Serial.println("");
  
  String str = "";
  str += _millis;  str += ",";
  str += imu.a.x;   str += ",";
  str += imu.a.y;   str += ",";
  str += imu.a.z;   str += ",";
  str += a;
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("acc_g.csv", buf);
}

void printdata(void)
{   
      Serial.print("!");

      #if PRINT_EULER == 1
      Serial.print("ANG:");
      Serial.print(ToDeg(roll));
      Serial.print(",");
      Serial.print(ToDeg(pitch));
      Serial.print(",");
      Serial.print(ToDeg(yaw));
      #endif      
      #if PRINT_ANALOGS==1
      Serial.print(",AN:");
      Serial.print(AN[0]);  //(int)read_adc(0)
      Serial.print(",");
      Serial.print(AN[1]);
      Serial.print(",");
      Serial.print(AN[2]);  
      Serial.print(",");
      Serial.print(AN[3]);
      Serial.print (",");
      Serial.print(AN[4]);
      Serial.print (",");
      Serial.print(AN[5]);
      Serial.print(",");
      Serial.print(c_magnetom_x);
      Serial.print (",");
      Serial.print(c_magnetom_y);
      Serial.print (",");
      Serial.print(c_magnetom_z);
      #endif
      #if PRINT_DCM == 1
      Serial.print (",DCM:");
      Serial.print(DCM_Matrix[0][0]);
      Serial.print (",");
      Serial.print(DCM_Matrix[0][1]);
      Serial.print (",");
      Serial.print(DCM_Matrix[0][2]);
      Serial.print (",");
      Serial.print(DCM_Matrix[1][0]);
      Serial.print (",");
      Serial.print(DCM_Matrix[1][1]);
      Serial.print (",");
      Serial.print(DCM_Matrix[1][2]);
      Serial.print (",");
      Serial.print(DCM_Matrix[2][0]);
      Serial.print (",");
      Serial.print(DCM_Matrix[2][1]);
      Serial.print (",");
      Serial.print(DCM_Matrix[2][2]);
      #endif
      Serial.println();
      
}

// 北を向くテスト
void turn_north(){
  Serial.println(yaw);
  while(yaw > 30){
    Serial.println(yaw);
    turn_left(80);
  }
  while(yaw < -30){
    Serial.println(yaw);
    turn_right(80);
  }
  motor_stop();
}

// 電熱線のテスト
void heat_test(){
  if(!digitalRead(sw)){
    beep(PUSHED);
    digitalWrite(heat1, HIGH);
    delay(5000);
    digitalWrite(heat1, LOW);
    delay(1000);
    digitalWrite(heat2, HIGH);
    delay(5000);
    digitalWrite(heat2, LOW);
    delay(1000);
  }
  
}

// テスト走行
void test_run(){
  turn_right(150);
  delay(4000);
  motor_stop();
  delay(2000);
  turn_right(255);
  delay(4000);
  motor_stop();
  delay(2000);
  forward(150);
  delay(4000);
  motor_stop();
  delay(2000);
  forward(255);
  delay(4000);
  motor_stop();
  delay(2000);
}
