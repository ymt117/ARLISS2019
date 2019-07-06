/*
 * References:
 *  - https://github.com/greiman/SdFat
 */

#include <SPI.h>
#include "SdFat.h"
SdFat SD;

//#define MYSD_SERIAL_DEBUG

#define SD_CS_PIN PB6
File file;

void sd_init(){
  Serial.print("Initializing SD card...");

  if(!SD.begin(SD_CS_PIN)){
    Serial.println("initialization failed!");
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
