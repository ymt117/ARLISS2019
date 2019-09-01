#include "STM32LowPower.h"
#include "cansat_define.h"

void low_power(){
    LowPower.begin();
    
    int low_power_count = 0;

    while(1){
        digitalWrite(led1, !digitalRead(led1));
        low_power_count++;
        if(low_power_count > 180){
            return;
        }
        LowPower.deepSleep(10000);
    }
}