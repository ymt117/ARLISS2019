#include <MadgwickAHRS.h>
#define MADGWICK_PRINT
//#define AAAA

#include "cansat_define.h"
#include "cansat_prototype.h"

Madgwick filter;

void madgwick_init(){
  filter.begin(10);
}

void madgwick_update(){
  //long m_time = millis();
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  
  //imu.read();
  //mag.read();
  Read_Gyro();
  Read_Accel();
  Read_Compass();

  // +-2000dps; 16bits; 70 mdps/LSB?
  //gx = (imu.g.x * 2000.0); gx /= 32768.0;
  //gy = (imu.g.y * 2000.0); gy /= 32768.0;
  //gz = (imu.g.z * 2000.0); gz /= 32768.0;
  gx = gyro_x; gx *= 2000.0; gx /= 32768.0;
  gy = gyro_y; gy *= 2000.0; gy /= 32768.0;
  gz = gyro_z; gz *= 2000.0; gz /= 32768.0;

  // +-16g; 16bits
  //ax = (imu.a.x * 16.0); ax /= 32768.0;
  //ay = (imu.a.y * 16.0); ay /= 32768.0;
  //az = (imu.a.z * 16.0); az /= 32768.0;
  ax = accel_x; ax *= 16.0; ax /= 32768.0;
  ay = accel_y; ay *= 16.0; ay /= 32768.0;
  az = accel_z; az *= 16.0; az /= 32768.0;

  // +-4gauss; 16bits
  //mx = (mag.m.x * 4.0) / 32768.0;
  //my = (mag.m.y * 4.0) / 32768.0;
  //mz = (mag.m.z * 4.0) / 32768.0;
  mx = mag.m.x / 6842;
  my = mag.m.y / 6842;
  mz = mag.m.z / 6842;
  
  filter.update(
    gx, gy, gz,
    ax, ay, az,
    mx, my, mz);

  roll = filter.getRoll();
  pitch = filter.getPitch();
  yaw = filter.getYaw();

#ifdef MADGWICK_PRINT
  Serial.print("Orientation: ");
  Serial.print(yaw);
  Serial.print(" ");
  Serial.print(pitch);
  Serial.print(" ");
  Serial.println(roll);
#endif

#ifdef AAAA
  Serial.print(gx); Serial.print("\t\t");
  Serial.print(gy); Serial.print("\t\t");
  Serial.print(gz); Serial.print("\t\t");
  //Serial.print(ax); Serial.print("\t\t");
  //Serial.print(ay); Serial.print("\t\t");
  //Serial.print(az); Serial.print("\t\t");
  //Serial.print(mag.m.x); Serial.print("\t");
  //Serial.print(mag.m.y); Serial.print("\t");
  //Serial.print(mag.m.z); Serial.print("\t");
  Serial.println();
#endif

  //Serial.println(millis() - m_time);
}
