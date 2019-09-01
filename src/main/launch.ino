#include "cansat_define.h"
#include "cansat_prototype.h"

// Macro for calc_altitude function
// 移動平均用のマクロ
#define NUM_MOVING_AVERAGE 3 // Number used for moving average

/***************************************************************
 * Calculate altitude from barometer value
 * 気圧センサから所得した値をもとに高度を求める関数
 **************************************************************/
float calc_altitude(){
    float altitude = 0.0;
    char buf[64];
    String str = "";

    // Use moving average
    // 移動平均を使う
    for(int i=0; i<NUM_MOVING_AVERAGE; i++){
        altitude += ps.pressureToAltitudeMeters(ps.readPressureMillibars());
        delay(20);
    }
    altitude = altitude / NUM_MOVING_AVERAGE;
    altitude = altitude - altitude_offset_value;
#ifdef CANSAT_SERIAL_DEBUG
    Serial.print("Altitude:\t\t"); Serial.print(altitude); Serial.println(" [m]");
#endif
    str = String("Altitude: "+String(altitude, 2)+" [m]\n");

    // Recoding to microSD card
    // システムログをmicroSDカードに記録する
    str.toCharArray(buf, str.length()+1);
    writeFile("/system_log.txt", buf);
    
    return altitude;
}

/***************************************************************
 * OFFSET for Altitude
 * 高度のオフセット値を求める
 **************************************************************/
void altitude_offset(){
    char buf[64];
    String str = "";

    altitude_offset_value = 0.0;

    // Measure altitude 50 times
    // 高度を50回求めて平均する
    for(int i=0; i<50; i++){
        altitude_offset_value += ps.pressureToAltitudeMeters(ps.readPressureMillibars());
        Serial.println(altitude_offset_value);
        delay(20);
    }
    Serial.print("altitude_offset_value: "); Serial.println(altitude_offset_value);
    altitude_offset_value = altitude_offset_value / 50;
#ifdef CANSAT_SERIAL_DEBUG
    Serial.print("Altitude OFFSET value:\t\t"); Serial.println(altitude_offset_value);
#endif
    str = String("Altitude OFFSET value: "+String(altitude_offset_value, 2)+"\n");

    // Recoding to microSD card
    // システムログをmicroSDカードに記録する
    str.toCharArray(buf, str.length()+1);
    writeFile("/system_log.txt", buf);
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
#ifdef CANSAT_SERIAL_DEBUG
            Serial.print("launch_count:\t\t"); Serial.print(launch_count); Serial.println(" [times]");
#endif
        }
    }

#ifdef CANSAT_SERIAL_DEBUG
    Serial.println("[!] launch detection judged by altitude");
#endif
    writeFile("/system_log.txt", "[!] launch detection judged by altitude");
    beep(LAUNCH_COMPLETE);
    s = State_release_detect;
    return;
}

/***************************************************************
 * State: Release detection
 *        Judgment based on [CdS] and [Time]
 **************************************************************/
void release_detect(){
    int release_count = 0;
    const int release_cds_threshold = 50;
    unsigned long release_time = millis();
    unsigned long release_wait = 180000;

    while(release_count < 10){

        writeAll();

        // Judge based on CdS
#ifdef CANSAT_SERIAL_DEBUG
        Serial.print("CdS value:\t\t"); Serial.println(analogRead(cds));
#endif
        if(analogRead(cds) < release_cds_threshold){
            release_count++;
#ifdef CANSAT_SERIAL_DEBUG
            Serial.print("release_count:\t\t"); Serial.print(release_count); Serial.println(" [times]");
#endif
        }

        // Judge based on Time
        if((millis() - release_time) > release_wait){// When [release_wait] minutes have passed
#ifdef CANSAT_SERIAL_DEBUG
            Serial.println("[!] release detection judged by time");
#endif
            writeFile("/system_log.txt", "[!] release detection judged by time");
            beep(RELEASE_COMPLETE);
            s = State_drop_detect;
            return;
        }

        delay(20);
    }
#ifdef CANSAT_SERIAL_DEBUG
    Serial.println("[!] release detection judged by cds");
#endif
    writeFile("/system_log.txt", "[!] release detection judged by cds");
    beep(RELEASE_COMPLETE);
    s = State_drop_detect;
    return;
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
    unsigned long drop_wait = 180000;

    while(gyro_flag == 0 || altitude_flag == 0){
        
        writeAll();

        // Judge based on Altitude
        if(calc_altitude() < drop_altitude_threshold){
            drop_count_altitude++;
#ifdef CANSAT_SERIAL_DEBUG
            Serial.print("drop_count_altitude:\t\t"); Serial.print(drop_count_altitude); Serial.println(" [times]");
#endif
            if(drop_count_altitude > 10) altitude_flag = 1;
        }

        // Judge based on Gyro
        imu.read();
        float gx = imu.g.x * 16; gx = gx / 32768;
        float gy = imu.g.y * 16; gy = gy / 32768;
        float gz = imu.g.z * 16; gz = gz / 32768;
#ifdef CANSAT_SERIAL_DEBUG
        Serial.print("Gyro\tX: "); Serial.print(gx, 4);
        Serial.print("\tY:"); Serial.print(gy, 4);
        Serial.print("\tZ: "); Serial.println(gz, 4);
#endif

        if(gx > -0.06 && gx < 0.06 && gy > -0.06 && gy < 0.06 && gz > -0.06 && gz < 0.06){
            drop_count_gyro++;
#ifdef CANSAT_SERIAL_DEBUG
            Serial.print("drop_count_gyro:\t\t"); Serial.print(drop_count_gyro); Serial.println(" [times]");
#endif
            if(drop_count_gyro > 10) gyro_flag = 1;
        }

        // Judge based on Time
        if((millis() - drop_time) > drop_wait){
#ifdef CANSAT_SERIAL_DEBUG
            Serial.println("[!] drop detection judged by time");
#endif
            writeFile("/system_log.txt", "[!] drop detection judged by time\n");
            beep(DROP_COMPLETE);
            s = State_first_fire;
            return;
        }

        delay(20);
    }

#ifdef CANSAT_SERIAL_DEBUG
    Serial.println("[!] drop detection judged by gyro and altitude");
#endif
    writeFile("/system_log.txt", "[!] drop detection judged by gyro and altitude");
    beep(DROP_COMPLETE);
    s = State_first_fire;
    return;
}