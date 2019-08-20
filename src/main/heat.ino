#include "cansat_define.h"
#include "cansat_prototype.h"


void _heat1(){
  beep(PUSHED);
  digitalWrite(heat1, HIGH);
  delay(3000);
  digitalWrite(heat1, LOW);
  delay(1000);
}

void _heat2(){
  beep(PUSHED);
  beep(PUSHED);
  digitalWrite(heat2, HIGH);
  delay(3000);
  digitalWrite(heat2, LOW);
  delay(1000);
}