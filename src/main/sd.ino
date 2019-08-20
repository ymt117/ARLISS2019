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

void readFile(const char * path){
  #ifdef MYSD_SERIAL_DEBUG
    Serial.print("Reading file: "); Serial.println(path);
  #endif
  
  file = SD.open(path, FILE_READ);

  if(file){
    #ifdef MYSD_SERIAL_DEBUG
      Serial.print("Reading to "); Serial.print(path);
    #endif

    while(file.available()){
      Serial.write(file.read());
    }
    file.close();
  }else{
    #ifdef MYSD_SERIAL_DEBUG
      Serial.print("error opening"); Serial.println(path);
    #endif
  }
}

void deleteFile(const char * path){
  #ifdef MYSD_SERIAL_DEBUG
    Serial.print("Deleting file: "); Serial.println(path);
  #endif

  SD.remove(path);

  if(SD.exists(path)){
    #ifdef MYSD_SERIAL_DEBUG
      Serial.print(path); Serial.println(" exists");
    #endif
  }else{
    #ifdef MYSD_SERIAL_DEBUG
      Serial.print(path); Serial.println(" doesn't exist");
    #endif
  }
}

int readStatus(const char * path){

  int my_status = -1;

  #ifdef MYSD_SERIAL_DEBUG
    Serial.print("Reading file: "); Serial.println(path);
  #endif
  
  file = SD.open(path, FILE_READ);

  if(file){
    #ifdef MYSD_SERIAL_DEBUG
      Serial.print("Reading to "); Serial.print(path);
    #endif

    if(file.available()){
      my_status = file.parseInt();
    }
    file.close();
  }else{
    #ifdef MYSD_SERIAL_DEBUG
      Serial.print("error opening"); Serial.println(path);
    #endif
  }

  return my_status;
}

void writeStatus(){
  char buf[32];

  deleteFile("status.txt");

  String str = "";
  str += s;

  int len = str.length();
  str.toCharArray(buf, len+1);
  
  writeFile("status.txt", buf);
}

void writeIMU(){
  char buf[1024];

  //Read_Gyro();
  //Read_Accel();
  //Read_Compass();
  imu.read();
  mag.read();

  String str = "";
  str += millis();    str += ",";
  str += imu.a.x;     str += ",";
  str += imu.a.y;     str += ",";
  str += imu.a.z;     str += ",";
  str += imu.g.x;     str += ",";
  str += imu.g.y;     str += ",";
  str += imu.g.z;     str += ",";
  str += mag.m.x;     str += ",";
  str += mag.m.y;     str += ",";
  str += mag.m.z;     str += ",";
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
  str += gps.date.year();                str += ",";
  str += gps.date.month();               str += ",";
  str += gps.date.day();                 str += ",";
  str += gps.time.hour();                str += ",";
  str += gps.time.minute();              str += ",";
  str += gps.time.second();              str += ",";
  str += String(gps.location.lat(), 6);  str += ",";
  str += String(gps.location.lng(), 6);  str += ",";
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("gps.csv", buf);
}

void writeCdS(){
  char buf[256];

  String str = "";
  str += millis();                str += ",";
  str += String(analogRead(cds)); str += ",";
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("cds.csv", buf);
}

void writeBattery(){
  char buf[256];

  String str = "";
  str += millis();                        str += ",";
  str += String(battery_voltage(batt1));  str += ",";
  str += String(battery_voltage(batt2));  str += ",";
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("battery.csv", buf);
}

void writeAll(){
  //long time = millis();
  char buf[1024];

  imu.read();
  mag.read();

  float pressure = ps.readPressureMillibars();
  float altitude = ps.pressureToAltitudeMeters(pressure);
  float temperature = ps.readTemperatureC();

  String str = "";
  str += millis();                       str += ",";
  str += s;                              str += ",";
  str += gps.date.year();                str += ",";
  str += gps.date.month();               str += ",";
  str += gps.date.day();                 str += ",";
  str += gps.time.hour();                str += ",";
  str += gps.time.minute();              str += ",";
  str += gps.time.second();              str += ",";
  str += String(gps.location.lat(), 6);  str += ",";
  str += String(gps.location.lng(), 6);  str += ",";
  str += imu.a.x;                        str += ",";
  str += imu.a.y;                        str += ",";
  str += imu.a.z;                        str += ",";
  str += imu.g.x;                        str += ",";
  str += imu.g.y;                        str += ",";
  str += imu.g.z;                        str += ",";
  str += mag.m.x;                        str += ",";
  str += mag.m.y;                        str += ",";
  str += mag.m.z;                        str += ",";
  str += pressure;                       str += ",";
  str += altitude;                       str += ",";
  str += temperature;                    str += ",";
  str += String(analogRead(cds));        str += ",";
  str += String(battery_voltage(batt1)); str += ",";
  str += String(battery_voltage(batt2)); str += ",";
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("log.csv", buf);
  
  //time = millis() - time;
  //Serial.println(time);
}