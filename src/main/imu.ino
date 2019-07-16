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
LIS3MDL::vector<int16_t> mag_min = {32767, 32767, 32767}, mag_max = {-32768, -32768, -32768};

char report[80];

/***************************************************************
 * Function of IMU Initialize
 **************************************************************/
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

/***************************************************************
 * Set IMU Offset
 **************************************************************/
 void imu_offset(){
  for(int i=0;i<32;i++)    // We take some readings...
    {
    Read_Gyro();
    Read_Accel();
    for(int y=0; y<6; y++)   // Cumulate values
      AN_OFFSET[y] += AN[y];
    delay(20);
    }

  for(int y=0; y<6; y++)
    AN_OFFSET[y] = AN_OFFSET[y]/32;

  AN_OFFSET[5]-=GRAVITY*SENSOR_SIGN[5];

  //Serial.println("Offset:");
  for(int y=0; y<6; y++)
    Serial.println(AN_OFFSET[y]);
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
  snprintf(report, sizeof(report), "6d\t%6d\t%6d\t",
    imu.g.x, imu.g.y, imu.g.z);
  Serial.print(report);

  // Show geomagnetism
  snprintf(report, sizeof(report), "6d\t%6d\t%6d\t",
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
  imu.read();
  
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
  imu.read();

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
  char buf[1024];

  magnetom_x = (SENSOR_SIGN[6] * mag.m.x) - MAG_OFFSET[6];
  magnetom_y = (SENSOR_SIGN[7] * mag.m.y) - MAG_OFFSET[7];
  magnetom_z = (SENSOR_SIGN[8] * mag.m.z) - MAG_OFFSET[8];

  String str = "";
  str += mag.m.x;       str += ",";
  str += mag.m.y;       str += ",";
  str += mag.m.z;       str += ",";
  str += magnetom_x;    str += ",";
  str += magnetom_y;    str += ",";
  str += magnetom_z;    str += ",";
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("mag.csv", buf);
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
 * Geomagnetic sensor calibration
 **************************************************************/
void compass_calibrate(){
  char buf[1024];
  unsigned long _millis = millis();

  digitalWrite(led1, HIGH);

  while(millis() - _millis < 30000){
    mag.read();
  
    MAG_OFFSET[1] = _min(MAG_OFFSET[1], mag.m.x);
    MAG_OFFSET[3] = _min(MAG_OFFSET[3], mag.m.y);
    MAG_OFFSET[5] = _min(MAG_OFFSET[5], mag.m.z);

    MAG_OFFSET[0] = _max(MAG_OFFSET[0], mag.m.x);
    MAG_OFFSET[2] = _max(MAG_OFFSET[2], mag.m.y);
    MAG_OFFSET[4] = _max(MAG_OFFSET[4], mag.m.z);
  }

  MAG_OFFSET[6] = (MAG_OFFSET[0] + MAG_OFFSET[1]) / 2; // (mx_max - mx_min) / 2
  MAG_OFFSET[7] = (MAG_OFFSET[2] + MAG_OFFSET[3]) / 2;
  MAG_OFFSET[8] = (MAG_OFFSET[4] + MAG_OFFSET[5]) / 2;

  String str = "";
  str += MAG_OFFSET[0];   str += ",";
  str += MAG_OFFSET[1];   str += ",";
  str += MAG_OFFSET[2];   str += ",";
  str += MAG_OFFSET[3];   str += ",";
  str += MAG_OFFSET[4];   str += ",";
  str += MAG_OFFSET[5];   str += ",";
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("mag_offset.csv", buf);

  digitalWrite(led1, LOW);
}
