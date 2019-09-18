#include "STM32LowPower.h"
#include "cansat_define.h"

// 10[s] * 180 = 1800[s] = 30[min]
// 10[s] * 360 = 3600[s] = 60[min]
// 10[s] * 540 = 5400[s] = 90[min]

void low_power(){
    LowPower.begin();
    
    int low_power_count = 0;

    while(1){
        digitalWrite(led1, !digitalRead(led1));
        low_power_count++;
        if(low_power_count > 540){
            return;
        }
        LowPower.deepSleep(10000);
    }
}