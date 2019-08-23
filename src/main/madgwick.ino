#include <MadgwickAHRS.h>
#define MADGWICK_PRINT

#include "cansat_define.h"
#include "cansat_prototype.h"

Madgwick filter;

void madgwick_init(){
  filter.begin(50);
}

void madgwick_update(){
  //long m_time = millis();

  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  unsigned long microsNow;
  
  // Check if it's time to read data and update the filter
  microsNow = micros();
  if(microsNow - microsPrevious >= microsPerReading){

    // Read data from IMU
    Read_Gyro();
    Read_Accel();
    // Read data from MAG
    Read_Compass();

    // Convert to gravity units
    ax = accel_x * 0.000488;
    ay = accel_y * 0.000488;
    az = accel_z * 0.000488;
    // Convert to degrees/second units
    gx = gyro_x * 0.07;
    gy = gyro_y * 0.07;
    gz = gyro_z * 0.07;
    // Convert to gauss
    mx = (magnetom_x - MAG_OFFSET[6]) * 0.000146;
    my = (magnetom_y - MAG_OFFSET[7]) * 0.000146;
    mz = (magnetom_z - MAG_OFFSET[8]) * 0.000146;

    // update the filter, which computes orientation
    filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    yaw = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(yaw);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);

    // increment pervious time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
  }

  //Serial.println(millis() - m_time);
}
