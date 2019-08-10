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

void _heat1(){
  beep(PUSHED);
  digitalWrite(heat1, HIGH);
  delay(3000);
  digitalWrite(heat1, LOW);
  delay(1000);
}

void _heat2(){
  beep(PUSHED);
  beep(PUSHED);
  digitalWrite(heat2, HIGH);
  delay(3000);
  digitalWrite(heat2, LOW);
  delay(1000);
}

void _motor_test(){
  beep(PUSHED);
  beep(PUSHED);
  beep(PUSHED);
  forward(100);
  delay(1000);
  motor_stop();
  delay(100);
}

// 電熱線のテスト（無線からON/OFF）
// 加振機のテストにも利用可
void heat_test2(){
  int val = 0;

  Read_Accel();
  Read_Gyro();
  Read_Compass();
  writeIMU();
  writeCdS();

  if(Serial.available() > 0){
    val = Serial.read();
    Serial.println(val);

    if(val == 120) _heat1();// 120 = x
    if(val == 121) _heat2();// 121 = y
    if(val == 122) _motor_test();// 122 = z
      
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

// TWE-Liteのスリープテスト
void twe_lite_sleep_test(){

  if(!digitalRead(sw)){
    beep(PUSHED);
    digitalWrite(twe_lite_sleep, !digitalRead(twe_lite_sleep));
  }

  Serial.println(analogRead(cds));
  delay(100);
}

// バッテリ電圧の監視
float battery_voltage(uint8_t pin){// batt1 or batt2
  
  return 3.3 / 1024 * 2 * analogRead(pin);
}

// 2点の緯度/経度から距離を計算
// https://keisan.casio.jp/exec/system/1257670779
double distance(float lng1, float lat1, float lng2, float lat2){// x1, y1, x2, y2
  double x1, y1, x2, y2;
  double d;
  double sin_y1, sin_y2, cos_y1, cos_y2, cos_dx, arc_cos;
  double delta_x;
  double r = 6378137;

  x1 = ToRad(lng1);
  y1 = ToRad(lat1);
  x2 = ToRad(lng2);
  y2 = ToRad(lat2);
  delta_x = x1 - x2;

  sin_y1 = sin(y1);
  sin_y2 = sin(y2);
  cos_y1 = cos(y1);
  cos_y2 = cos(y2);
  cos_dx = cos(delta_x);

  arc_cos = acos(sin_y1 * sin_y2 + cos_y1 * cos_y2 * cos_dx);

  d = r * arc_cos;

  return d;
}

// 2点の緯度/経度から方位を計算
// http://kwikwi.cocolog-nifty.com/blog/2005/12/delphi_2_fd2c.html
double direction(float lng1, float lat1, float lng2, float lat2){
  double x1, y1, x2, y2;
  double phi;
  double X, Y;
  double sin_y1, sin_y2, cos_y1, cos_y2;
  double sin_dx, cos_dx;
  double delta_x;

  x1 = ToRad(lng1);
  y1 = ToRad(lat1);
  x2 = ToRad(lng2);
  y2 = ToRad(lat2);
  delta_x = x2 - x1;

  sin_y1 = sin(y1);
  sin_y2 = sin(y2);
  cos_y1 = cos(y1);
  cos_y2 = cos(y2);
  sin_dx = sin(delta_x);
  cos_dx = cos(delta_x);

  Y = cos_y2 * sin_dx;
  X = cos_y1 * sin_y2 - sin_y1 * cos_y2 * cos_dx;

  phi = atan2(Y, X);

  phi = ToDeg(phi);
  if(phi < 0) phi += 360;

  return phi;
}

// 現在地とゴールの距離，方位の計算テスト
void dd_test(){
  while(hs.available() > 0)
    gps.encode(hs.read());

  if(gps.location.isUpdated()){
    Serial.print("Distance: ");
    Serial.print(distance(g_lng, g_lat, gps.location.lng(), gps.location.lat()));
    Serial.print("\t");
    Serial.print("Direction: ");
    Serial.print(direction(g_lng, g_lat, gps.location.lng(), gps.location.lat()));
    Serial.println();
  }
  //Serial.print("yaw: ");
  //Serial.print(yaw);

  //Serial.println();

  writeGPS();
  writeIMU();
}

// 電力テスト
void power_test(){
  forward(255);
  while(1){
    writeAll();
    gps_read();
    delay(100);
  }
}