#include <MadgwickAHRS.h>
#define MADGWICK_PRINT

Madgwick filter;

void madgwick_init(){
  filter.begin(100);
}

void madgwick_update(){
  //long m_time = millis();
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  
  Read_Gyro();
  Read_Accel();
  //Read_Compass();

  gx = (gyro_x * 16.0) / 32768.0;
  gy = (gyro_y * 16.0) / 32768.0;
  gz = (gyro_z * 16.0) / 32768.0;

  ax = (accel_x * 245.0) / 32768.0;
  ay = (accel_y * 245.0) / 32768.0;
  az = (accel_z * 245.0) / 32768.0;

  mx = (magnetom_x * 4.0) / 2048.0;
  my = (magnetom_y * 4.0) / 2048.0;
  mz = (magnetom_z * 4.0) / 2048.0;
  
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

  //Serial.println(millis() - m_time);
}
