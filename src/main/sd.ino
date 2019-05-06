#include <SPI.h>
#include "SdFat.h"
SdFat SD;

#define SD_CS_PIN PA15
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
  Serial.print("Writing file: "); Serial.println(path);

  file = SD.open(path, FILE_WRITE);

  if(file){
    Serial.print("Writing to "); Serial.print(path);
    file.println(message);
    file.close();
    Serial.println(" done.");
  }else{
    Serial.println("error opening"); Serial.print(path);
  }
}
