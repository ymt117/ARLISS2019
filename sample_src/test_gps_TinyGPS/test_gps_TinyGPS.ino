#include <TinyGPS++.h>
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
HardwareSerial usart1(USART1);

void setup() {
  Serial.begin(9600);
  usart1.begin(GPSBaud);

  Serial.println(F("GPS test start!"));
}

void loop() {
  while(usart1.available() > 0){
    //Serial.println("+");
    char c = usart1.read();
    gps.encode(c);
    if(gps.location.isUpdated()){
      Serial.print("Lat=\t");   Serial.print(gps.location.lat(), 6);
      Serial.print("\tLng=\t");   Serial.print(gps.location.lng(), 6);
      Serial.print("\tAlt=\t");   Serial.println(gps.altitude.meters());
    }
  }
  //Serial.println("#");
}
