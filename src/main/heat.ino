#include "cansat_define.h"
#include "cansat_prototype.h"

#define HEATING_TIME 5000

void heat_init(){
  
  pinMode(heat1, OUTPUT);
  digitalWrite(heat1, LOW);

  pinMode(heat2, OUTPUT);
  digitalWrite(heat2, LOW);
}

void _heat1(){
  //beep(PUSHED);
  digitalWrite(heat1, HIGH);
  delay(HEATING_TIME);
  digitalWrite(heat1, LOW);
  delay(1000);
}

void _heat2(){
  //beep(PUSHED);
  //beep(PUSHED);
  digitalWrite(heat2, HIGH);
  delay(HEATING_TIME);
  digitalWrite(heat2, LOW);
  delay(1000);
}