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

void Read_Accel(){
  imu.read();

  AN[3] = imu.a.x;
  AN[4] = imu.a.y;
  AN[5] = imu.a.z;

  accel_x = SENSOR_SIGN[3] * (AN[3] - AN_OFFSET[3]);
  accel_y = SENSOR_SIGN[4] * (AN[4] - AN_OFFSET[4]);
  accel_z = SENSOR_SIGN[5] * (AN[5] - AN_OFFSET[5]);
}

void Read_Compass(){
  mag.read();

  magnetom_x = SENSOR_SIGN[6] * mag.m.x;
  magnetom_y = SENSOR_SIGN[7] * mag.m.x;
  magnetom_z = SENSOR_SIGN[8] * mag.m.x;
}

void imu_test(){
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

    //Serial.print("ANG:");
    Serial.print(ToDeg(roll));
    Serial.print("\t");
    Serial.print(ToDeg(pitch));
    Serial.print("\t");
    Serial.print(ToDeg(yaw));
    Serial.println();
  }
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
void mag_calibrate(){
  char buf[1024];
  unsigned long _millis = millis();

  while(millis() - _millis < 10000){
    mag.read();
    turn_left(100);
    
    String str = "";
    str += millis();   str += ",";
    str += mag.m.x;   str += ",";
    str += mag.m.y;   str += ",";
    str += mag.m.z;   str += ",";
    str += "\n";

    int len = str.length();
    str.toCharArray(buf, len+1);

    writeFile("mag.csv", buf);
  }
  
  //motor_stop();
  //delay(1000);
}
