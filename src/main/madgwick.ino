#include <MadgwickAHRS.h>

Madgwick filter;

void madgwick_init(){
  filter.begin(10);
}

void test_madgwick(){
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float madgwick_roll, madgwick_pitch, madgwick_heading;
  
  Read_Gyro();
  Read_Accel();
  Read_Compass();

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

  madgwick_roll = filter.getRoll();
  madgwick_pitch = filter.getPitch();
  madgwick_heading = filter.getYaw();

  //Serial.print(mx); Serial.print("\t");
  //Serial.print(my); Serial.print("\t");
  //Serial.println(mz);

  Serial.print("Orientation: ");
  Serial.print(madgwick_heading);
  Serial.print(" ");
  Serial.print(madgwick_pitch);
  Serial.print(" ");
  Serial.println(madgwick_roll);
}
