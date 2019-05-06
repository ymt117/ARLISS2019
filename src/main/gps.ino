#include <TinyGPS++.h>

TinyGPSPlus gps;
HardwareSerial hs(PA10, PA9);

void gps_init(){
  hs.begin(9600);
  Serial.println("GPS start!");
}

void gps_read(){
  while(hs.available() > 0)
    gps.encode(hs.read());

  if(gps.location.isUpdated()){
    Serial.print("Lat=\t");   Serial.print(gps.location.lat(), 6);
    Serial.print("\tLng=\t");   Serial.print(gps.location.lng(), 6);
    Serial.print("\tAlt=\t");   Serial.println(gps.altitude.meters());
  }
}

void gps_plain_text(){
  if(hs.available())
    Serial.write(hs.read());
}
