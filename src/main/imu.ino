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

LSM6 imu;
LPS ps;
LIS3MDL mag;

char report[80];

void imu_init(){
  Wire.begin();
  
  if(!imu.init()){
    Serial.println("Failed to detect and initialize IMU!");
    while(1) led1_blink_fast();
  }
  imu.enableDefault();

  if(!ps.init()){
    Serial.println("Failed to autodetect pressure sensor!");
    while(1) led1_blink_fast();
  }
  ps.enableDefault();

  if(!mag.init()){
    Serial.println("Failed to detect and initialize magnetometer!");
    while(1) led1_blink_fast();
  }
  mag.enableDefault();
}

void imu_read(){
  imu.read();
  mag.read();
  float pressure = ps.readPressureMillibars();
  float temperature = ps.readTemperatureC();

  snprintf(report, sizeof(report), "A: %6d %6d %6d   G: %6d %6d %6d   M: %6d %6d %6d",
    imu.a.x, imu.a.y, imu.a.z,
    imu.g.x, imu.g.y, imu.g.z,
    mag.m.x, mag.m.y, mag.m.z);
  Serial.print(report);
  
  Serial.print(" p: ");
  Serial.print(pressure);
  Serial.print(" t: ");
  Serial.print(temperature);

  Serial.println("");

  delay(100);
}

void imu_plot(){
  imu.read();
  mag.read();
  float pressure = ps.readPressureMillibars();
  float temperature = ps.readTemperatureC();

  snprintf(report, sizeof(report), "%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d",
    imu.a.x, imu.a.y, imu.a.z,
    imu.g.x, imu.g.y, imu.g.z,
    mag.m.x, mag.m.y, mag.m.z);
  Serial.print(report);
  
  //Serial.print("\t");
  //Serial.print(pressure);
  //Serial.print("\t");
  //Serial.print(temperature);

  Serial.println("");

  delay(100);
}

float ax_g(){
  imu.read();

  // X軸にかかるGを求める
  float ax_g = imu.a.x * 16;
  ax_g /= 32768;

  return ax_g;
}

float ay_g(){
  imu.read();

  // Y軸にかかるGを求める
  float ay_g = imu.a.y * 16;
  ay_g /= 32768;

  return ay_g;
}

float az_g(){
  imu.read();

  // Z軸にかかるGを求める
  float az_g = imu.a.z * 16;
  az_g /= 32768;

  return az_g;
}

float calc_a(float ax, float ay, float az){
  ax *= 16;
  ax /= 32768;
  ay *= 16;
  ay /= 32768;
  az *= 16;
  az /= 32768;

  return sqrt(ax*ax + ay*ay + az*az);
}
