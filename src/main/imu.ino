/*
 * References:
 *  - https://github.com/pololu/lsm6-arduino
 *  - https://github.com/pololu/lis3mdl-arduino
 *  - https://github.com/pololu/lps-arduino
 */

#include <Wire.h>
#include <LSM6.h>
#include <LPS.h>
#include <LIS3MDL.h>

#include "cansat_define.h"
#include "cansat_prototype.h"

LSM6 imu;
LPS ps;
LIS3MDL mag;
LIS3MDL::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = {-32768, -32768, -32768};

char report[80];

/***************************************************************
 * Function of IMU Initialize
 **************************************************************/
void imu_init(){
  Wire.begin();

  Serial.print("IMU initialization ... ");
  
  if(!imu.init()){
    Serial.println("Failed to detect and initialize IMU!");
    beep(CANSAT_ERROR2);
    return;
  }
  imu.enableDefault();

  if(!ps.init()){
    Serial.println("Failed to autodetect pressure sensor!");
    return;
  }
  ps.enableDefault();

  if(!mag.init()){
    Serial.println("Failed to detect and initialize magnetometer!");
    return;
  }
  mag.enableDefault();

  Serial.println("done!");
  imu_status = 1;
}

/***************************************************************
 * Set IMU Offset
 **************************************************************/
void imu_offset(){
  // Returns if initialization fails
  if(imu_status == -1) return;
  
  for(int i=0;i<32;i++){// We take some readings...
    Read_Gyro();
    Read_Accel();
    for(int y=0; y<6; y++)// Cumulate values
      AN_OFFSET[y] += AN[y];
    delay(20);
  }

  for(int y=0; y<6; y++)
    AN_OFFSET[y] = AN_OFFSET[y]/32;

  AN_OFFSET[5]-=GRAVITY*SENSOR_SIGN[5];

  //Serial.println("Offset:");
  //for(int y=0; y<6; y++)
    //Serial.println(AN_OFFSET[y]);
 }

/***************************************************************
 * Function of Serial Plot IMU data
 **************************************************************/
void imu_plot(){
  imu.read();
  mag.read();
  float pressure = ps.readPressureMillibars();
  float temperature = ps.readTemperatureC();

  // Show acceleration
  snprintf(report, sizeof(report), "%6d\t%6d\t%6d\t",
    imu.a.x, imu.a.y, imu.a.z);
  Serial.print(report);

  // Show gyro
  snprintf(report, sizeof(report), "%6d\t%6d\t%6d\t",
    imu.g.x, imu.g.y, imu.g.z);
  Serial.print(report);

  // Show geomagnetism
  snprintf(report, sizeof(report), "%6d\t%6d\t%6d\t",
    mag.m.x, mag.m.y, mag.m.z);
  Serial.print(report);

  Serial.println("");

  delay(100);
}

/***************************************************************
 * Function: Read Gyro, Accel and Compass
 **************************************************************
 * ReadGyro
 **************************************************************/
void Read_Gyro(){
  imu.readGyro();
  
  AN[0] = imu.g.x;
  AN[1] = imu.g.y;
  AN[2] = imu.g.z;

  gyro_x = SENSOR_SIGN[0] * (AN[0] - AN_OFFSET[0]);
  gyro_y = SENSOR_SIGN[1] * (AN[1] - AN_OFFSET[1]);
  gyro_z = SENSOR_SIGN[2] * (AN[2] - AN_OFFSET[2]);
}
/***************************************************************
 * Read Accel
 **************************************************************/
void Read_Accel(){
  imu.readAcc();

  AN[3] = imu.a.x;
  AN[4] = imu.a.y;
  AN[5] = imu.a.z;

  accel_x = SENSOR_SIGN[3] * (AN[3] - AN_OFFSET[3]);
  accel_y = SENSOR_SIGN[4] * (AN[4] - AN_OFFSET[4]);
  accel_z = SENSOR_SIGN[5] * (AN[5] - AN_OFFSET[5]);
}
/***************************************************************
 * Read Compass
 **************************************************************/
void Read_Compass(){
  mag.read();

  magnetom_x = SENSOR_SIGN[6] * mag.m.x;
  magnetom_y = SENSOR_SIGN[7] * mag.m.y;
  magnetom_z = SENSOR_SIGN[8] * mag.m.z;
}


/***************************************************************
 * Calculate G-force
 **************************************************************/
float calc_g_force(float ax, float ay, float az){
  ax *= 16; ax /= 32768; // ax = ax × 16 ÷ 32768
  ay *= 16; ay /= 32768; // ay = ay × 16 ÷ 32768
  az *= 16; az /= 32768; // az = az × 16 ÷ 32768

  return sqrt(ax*ax + ay*ay + az*az); // √(ax^2 + ay^2 + az^2)
}


/***************************************************************
 * IMU AHRS UPDATE
 * Runs at 50Hz(Called every 20ms)
 **************************************************************/
void imu_ahrs_update(){

  if((millis()-timer)>=20)  // Main loop runs at 50Hz
  {
    counter++;
    timer_old = timer;
    timer=millis();
    if (timer>timer_old)
    {
      G_Dt = (timer-timer_old)/1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
      if (G_Dt > 0.2)
        G_Dt = 0; // ignore integration times over 200 ms
    }
    else
      G_Dt = 0;
      

    // *** DCM algorithm
    // Data adquisition
    Read_Gyro();   // This read gyro data
    Read_Accel();     // Read I2C accelerometer

    if (counter > 5)  // Read compass data at 10Hz... (5 loop runs)
    {
      counter=0;
      Read_Compass();    // Read I2C magnetometer
      Compass_Heading(); // Calculate magnetic heading
    }

    // Calculations...
    Matrix_update();
    Normalize();
    Drift_correction();
    Euler_angles();
    // ***
    
    Serial.print("Orientation: ");
    Serial.print(ToDeg(yaw));
    Serial.print(" ");
    Serial.print(ToDeg(pitch));
    Serial.print(" ");
    Serial.println(ToDeg(roll));
  }
}

/***************************************************************
 * Compass calibration 2D
 **************************************************************/
void compass_calibration_2d(){
  unsigned long start_time = millis();
  int calibration_time = 7500;
  char buf[256];
  String str = "";

  // Calibration
  Serial.print("[!] Compass calibration in xy coordinates ... ");
  // Generate a character string to be recorded on the SD card
  // microSDカードに記録する文字列を生成する
  str += String("[!] Compass calibration in xy coordinates ... ");

  right_cw(255);
  digitalWrite(led1, HIGH);
  while((millis() - start_time) < calibration_time){
    Read_Compass();

    running_min.x = _min(running_min.x, magnetom_x);
    running_min.y = _min(running_min.y, magnetom_y);

    running_max.x = _max(running_max.x, magnetom_x);
    running_max.y = _max(running_max.y, magnetom_y);
  }
  digitalWrite(led1, LOW);
  beep(PUSHED);

  motor_stop();
  delay(2000);

  start_time = millis();

  left_ccw(255);
  digitalWrite(led1, HIGH);
  while((millis() - start_time) < calibration_time){
    Read_Compass();

    running_min.x = _min(running_min.x, magnetom_x);
    running_min.y = _min(running_min.y, magnetom_y);

    running_max.x = _max(running_max.x, magnetom_x);
    running_max.y = _max(running_max.y, magnetom_y);
  }
  digitalWrite(led1, LOW);
  beep(PUSHED);

  Serial.println("done");
  str += String("done\n");
  Serial.print("MAG_X_OFFSET: "); Serial.print((running_max.x + running_min.x) * 0.5);
  Serial.print("\tMAG_Y_OFFSET: "); Serial.println((running_max.y + running_min.y) * 0.5);
  str += String("MAG_X_OFFSET: "+String((running_max.x + running_max.x) * 0.5));
  str += String("MAG_Y_OFFSET: "+String((running_max.y + running_max.y) * 0.5));

  motor_stop();
  delay(2000);

  // Recoding to microSD card
  // システムログをmicroSDカードに記録する
  int len = str.length();
  str.toCharArray(buf, len+1);
  writeFile("/system_log.txt", buf);
}

/***************************************************************
 * Compass Heading 2D
 **************************************************************/
void compass_heading_2d(){

  // Calculation heading
  // Recode your magnetom value 10 times
  // 地磁気センサの値を10回記録して平均する
  for(int i=0; i<10; i++){
    Read_Compass();
    c_magnetom_x += magnetom_x - ((running_max.x + running_min.x) * 0.5);
    c_magnetom_y += magnetom_y - ((running_max.y + running_min.y) * 0.5);
  }
  c_magnetom_x = c_magnetom_x / 10;
  c_magnetom_y = c_magnetom_y / 10;

  MAG_Heading = atan2(-c_magnetom_y, c_magnetom_x);
  Serial.print("Heading:\t"); Serial.println(ToDeg(MAG_Heading));
}