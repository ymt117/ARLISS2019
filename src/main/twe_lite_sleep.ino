// Sleep function by M3 pin
#include "cansat_define.h"
#include "cansat_prototype.h"

void twe_lite_wakeup(){

    digitalWrite(twe_lite_sleep_pin, HIGH);
}

void twe_lite_sleep(){

    digitalWrite(twe_lite_sleep_pin, LOW);
}