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

void _motor_test(){
  beep(PUSHED);
  beep(PUSHED);
  beep(PUSHED);
  //forward(100);
  right_cw(100);
  left_ccw(100);
  delay(1000);
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, LOW);
  digitalWrite(pwma, LOW);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, LOW);
  digitalWrite(pwmb, LOW);
  //motor_stop();
  delay(100);
}

// 電熱線のテスト（無線からON/OFF）
// 加振機のテストにも利用可
void heat_test2(){
  int val = 0;

  writeAll();

  if(Serial.available() > 0){
    val = Serial.read();
    Serial.println(val);

    if(val == 113) _heat1();// 120 = x, 113 = q
    if(val == 118) _heat2();// 121 = y, 118 = v
    if(val == 119) _motor_test();// 122 = z, 119 = w
      
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

  //writeGPS();
  //writeIMU();
}

// 電力テスト
void power_test(){
  int voltage_count = 0;
  long time = millis();

  while(s == State_test){

    // Send GPS values every minute
    if((millis() - time) > 60000){
      time = millis();
      gps_read();
    }

    // Stop when 3.5V or less
    if((battery_voltage(batt1) <= 3.5) | battery_voltage(batt2) <= 3.5){
      voltage_count++;
      if(voltage_count > 10) s = State_exit;
    }

    forward(150);

    writeAll();
    delay(100);
  }
}

// ファイルの書き込み読み込みテスト
void read_write_test(){

  int read_status = -2;

  // -2
  Serial.print("read_status: ");
  Serial.println(read_status);

  read_status = readStatus("status.txt");
  Serial.print("read_status: ");
  Serial.println(read_status);
  

  delay(100);

  Serial.println("Writing status.txt");
  writeStatus();

  delay(100);

  // 1
  read_status = readStatus("status.txt");
  Serial.print("read_status: ");
  Serial.println(read_status);

  delay(100);

  Serial.println("Writing status.txt");
  writeStatus();

  delay(100);

  // 1
  read_status = readStatus("status.txt");
  Serial.print("read_status: ");
  Serial.println(read_status);

  delay(100);
}

// 地磁気でHeadingのテスト
void heading_test(){
  compass_calibrate();

  timer_init_3();

  while(1){
    //Read_Compass();
    //Compass_Heading();
    //Serial.print("Heading RAD: "); Serial.print(MAG_Heading);
    //Serial.print("\tDEG: "); Serial.println(ToDeg(MAG_Heading));
  }
}

void sw_motor(){
  delay(2000);
  forward(255);// f
  delay(2000);
  motor_stop();// s
  delay(2000);
  right_ccw(255);// rccw
  delay(2000);
  motor_stop(); // s
  delay(2000);
  right_cw(255); // rcw
  delay(2000);
  motor_stop();// s
  delay(2000);
  left_ccw(255); // lccw
  delay(2000);
  motor_stop(); // s
  delay(2000);
  left_cw(255); // lcw
  delay(2000);
  motor_stop(); // s
  delay(2000);

  delay(2000);
  forward(100);// f
  delay(2000);
  motor_stop();// s
  delay(2000);
  right_ccw(100);// rccw
  delay(2000);
  motor_stop(); // s
  delay(2000);
  right_cw(100); // rcw
  delay(2000);
  motor_stop();// s
  delay(2000);
  left_ccw(100); // lccw
  delay(2000);
  motor_stop(); // s
  delay(2000);
  left_cw(100); // lcw
  delay(2000);
  motor_stop(); // s
  delay(2000);
}

//====================================================
//直進　（ジャイロ）　→　（偏角）　→　（PWM）
//
//Gyro:2000dps ODR:1.66[khz] (enabledDefault)
//
//loop_num (1/ about 30（+α）ms)
//====================================================
int thz = 0;

void gost(int m_second){
  
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