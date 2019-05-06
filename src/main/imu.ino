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
    while(1);
  }
  imu.enableDefault();

  if(!ps.init()){
    Serial.println("Failed to autodetect pressure sensor!");
    while(1);
  }
  ps.enableDefault();

  if(!mag.init()){
    Serial.println("Failed to detect and initialize magnetometer!");
    while(1);
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
