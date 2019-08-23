#include "cansat_define.h"
#include "cansat_prototype.h"

// Macro for calc_altitude function
#define NUM_MOVING_AVERAGE 3 // Number used for moving average

/***************************************************************
 * Calculate altitude from barometer value
 **************************************************************/
float calc_altitude(){
    float altitude = 0.0;

    // Use moving average
    for(int i=0; i<NUM_MOVING_AVERAGE; i++){
        altitude += ps.pressureToAltitudeMeters(ps.readPressureMillibars());
        delay(20);
    }
    altitude = altitude / NUM_MOVING_AVERAGE;
    altitude -= altitude_offset_value;

    Serial.print("Altitude:\t\t"); Serial.print(altitude); Serial.println(" [m]");
    
    return altitude;
}

/***************************************************************
 * OFFSET for Altitude
 **************************************************************/
void altitude_offset(){

    // Measure altitude 50 times
    for(int i=0; i<50; i++){
        altitude_offset_value += ps.pressureToAltitudeMeters(ps.readPressureMillibars());
        delay(20);
    }
    altitude_offset_value = altitude_offset_value / 50;

    Serial.print("Altitude OFFSET value:\t\t"); Serial.println(altitude_offset_value);
}

/***************************************************************
 * State: Launch detection
 *        Judgment based on [Altitude]
 **************************************************************/
void launch_detect(){
    int launch_count = 0;
    int launch_altitude_threshold = 5;
    
    while(launch_count < 10){// 10 times or more until reaching [launch_altitude] meters

        writeAll();

        // Judge based on Altitude
        if(calc_altitude() > launch_altitude_threshold){// If the altitude exceeds [launch_altitude_threshold] meters
            launch_count++;
            Serial.print("launch_count:\t\t"); Serial.print(launch_count); Serial.println(" [times]");
        }
    }

    beep(LAUNCH_COMPLETE);
    s = State_release_detect;
}

/***************************************************************
 * State: Release detection
 *        Judgment based on [CdS] and [Time]
 **************************************************************/
void release_detect(){
    int release_count = 0;
    const int release_cds_threshold = 50;
    unsigned long release_time = millis();
    unsigned long release_wait = 18000;

    while(release_count < 10){

        writeAll();

        // Judge based on CdS
        Serial.print("CdS value:\t\t"); Serial.println(analogRead(cds));
        if(analogRead(cds) < release_cds_threshold){
            release_count++;
            Serial.print("release_count:\t\t"); Serial.print(release_count); Serial.println(" [times]");
        }

        // Judge based on Time
        if((millis() - release_time) > release_wait){// When [release_wait] minutes have passed
            beep(RELEASE_COMPLETE);
            break;
        }

        delay(20);
    }

    beep(RELEASE_COMPLETE);
    s = State_drop_detect;
}

/***************************************************************
 * State: Drop detection
 *        Judgment based on [Altitude] and [Gyro] and [Time]
 **************************************************************/
void drop_detect(){

    int drop_count_gyro = 0;
    int drop_count_altitude = 0;
    int drop_altitude_threshold = 1;
    int gyro_flag = 0;
    int altitude_flag = 0;
    unsigned long drop_time = millis();
    unsigned long drop_wait = 18000;

    while(gyro_flag == 0 || altitude_flag == 0){
        
        writeAll();

        // Judge based on Altitude
        if(calc_altitude() < drop_altitude_threshold){
            drop_count_altitude++;
            Serial.print("drop_count_altitude:\t\t"); Serial.print(drop_count_altitude); Serial.println(" [times]");
            if(drop_count_altitude > 10) altitude_flag = 1;
        }

        // Judge based on Gyro
        imu.read();
        float gx = imu.g.x * 16; gx = gx / 32768;
        float gy = imu.g.y * 16; gy = gy / 32768;
        float gz = imu.g.z * 16; gz = gz / 32768;
        Serial.print("Gyro\tX: "); Serial.print(gx, 4);
        Serial.print("\tY:"); Serial.print(gy, 4);
        Serial.print("\tZ: "); Serial.println(gz, 4);

        if(gx > -0.06 && gx < 0.06 && gy > -0.06 && gy < 0.06 && gz > -0.06 && gz < 0.06){
            drop_count_gyro++;
            Serial.print("drop_count_gyro:\t\t"); Serial.print(drop_count_gyro); Serial.println(" [times]");
            if(drop_count_gyro > 10) gyro_flag = 1;
        }

        // Judge based on Time
        if((millis() - drop_time) > drop_wait){
            beep(DROP_COMPLETE);
            break;
        }

        delay(20);
    }

    beep(DROP_COMPLETE);
    s = State_first_fire;
}