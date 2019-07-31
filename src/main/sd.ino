/*
 * References:
 *  - https://github.com/greiman/SdFat
 */

#include <SPI.h>
#include "SdFat.h"
SdFat SD;

#include "cansat_define.h"
#include "cansat_prototype.h"

//#define MYSD_SERIAL_DEBUG

#define SD_CS_PIN PB6
File file;

void sd_init(){
  Serial.print("Initializing SD card...");

  if(!SD.begin(SD_CS_PIN)){
    Serial.println("initialization failed!");
    beep(CANSAT_ERROR);
    return;
  }
  Serial.println("initialization done.");
}

void writeFile(const char * path, const char * message){
  #ifdef MYSD_SERIAL_DEBUG
    Serial.print("Writing file: "); Serial.println(path);
  #endif

  file = SD.open(path, FILE_WRITE);

  if(file){
    #ifdef MYSD_SERIAL_DEBUG
      Serial.print("Writing to "); Serial.print(path);
    #endif
    
    file.print(message);
    file.close();
    
    #ifdef MYSD_SERIAL_DEBUG
      Serial.println(" done.");
    #endif
  }else{
    #ifdef MYSD_SERIAL_DEBUG
      Serial.println("error opening"); Serial.print(path);
    #endif
  }
}

void writeIMU(){
  char buf[1024];

  //Read_Gyro();
  //Read_Accel();
  //Read_Compass();

  String str = "";
  str += millis();      str += ",";
  str += accel_x;       str += ",";
  str += accel_y;       str += ",";
  str += accel_z;       str += ",";
  str += gyro_x;        str += ",";
  str += gyro_y;        str += ",";
  str += gyro_z;        str += ",";
  str += magnetom_x;    str += ",";
  str += magnetom_y;    str += ",";
  str += magnetom_z;    str += ",";
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("imu.csv", buf);
}

void writeLPS(){
  char buf[512];
  float pressure = ps.readPressureMillibars();
  float altitude = ps.pressureToAltitudeMeters(pressure);
  float temperature = ps.readTemperatureC();

  String str = "";
  str += millis();    str += ",";
  str += pressure;    str += ",";
  str += altitude;    str += ",";
  str += temperature; str += ",";
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("lps.csv", buf);
}

void writeGPS(){
  char buf[512];

  String str = "";
  str += millis();                       str += ",";
  str += String(gps.location.lat(), 6);  str += ",";
  str += String(gps.location.lng(), 6);  str += ",";
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("gps.csv", buf);
}
