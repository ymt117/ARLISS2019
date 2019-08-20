#include "cansat_define.h"

#define NUM_MOVING_AVERAGE 3

// altitude_offset
float pressure;
float altitud;
float temperature;
int al_offset_cnt = 0;
float offset_a[50] = {0.0};
float average_altitude_offset = 0.0;       //基準の高さの平均 
float sum_offset = 0.0;       //基準の高さの合計
int repeat_num = 50;          //基準の高さを求めるのに取るデータの数


float calc_altitude(){
    float altitude[NUM_MOVING_AVERAGE];
    float sum_altitude = 0.0;

    for(int i=0; i<NUM_MOVING_AVERAGE; i++){
        altitude[i] = ps.pressureToAltitudeMeters(ps.readPressureMillibars());
        Serial.print("altitude: "); Serial.println(altitude[i] - average_altitude_offset);
    }

    for(int i=0; i<NUM_MOVING_AVERAGE; i++){
        sum_altitude += altitude[i];
    }

    float average_altitude = sum_altitude / NUM_MOVING_AVERAGE;

    return average_altitude - average_altitude_offset;
}

void altitude_offset(){
    Serial.println("altitude_offset");

    //最初に測った高度を基準とする --->複数の値から基準を求める
    while(al_offset_cnt < repeat_num) {
        delay(20);
        pressure = ps.readPressureMillibars();           //気圧
        altitud = ps.pressureToAltitudeMeters(pressure); //高度

        Serial.print("altitude="); Serial.println(altitud);
        offset_a[al_offset_cnt] = altitud;    //測定した高度を配列offset_aの中に入れる
        sum_offset += offset_a[al_offset_cnt]; //速度の合計sum_offsetの中に配列offset_aの値を入れる
        al_offset_cnt++;
    }
    Serial.println("finish altitude offset");
    average_altitude_offset = sum_offset / repeat_num; //基準高度を算出
}

int launch_count = 0;

void launch_detect(){
    if(calc_altitude() > 5){
        launch_count++;
    }

    if(launch_count > 10){
        s = State_release_detect;
    }
}

const int cds_threshold = 300;
int release_count = 0;

void release_detect(){
    if(analogRead(cds) < cds_threshold){
        release_count++;
    }

    if(release_count > 10){
        s = State_drop_detect;
    }
}

int drop_count_pressure = 0;
int drop_count_gyro = 0;
int drop_flag = 0;

void drop_detect(){
    long drop_detect_time = millis();

    while((millis() - drop_detect_time) < 3){
        // pressure
        if(calc_altitude() < 1){
            drop_count_pressure++;
            Serial.print("drop_count_pressure: "); Serial.println(drop_count_pressure);
        }

        // gyro
        imu.read();

        float gx = imu.g.x * 2; gx = gx / 32768;
        float gy = imu.g.y * 2; gy = gy / 32768;
        float gz = imu.g.z * 2; gz = gz / 32768;

        Serial.print("gx: "); Serial.print(gx);
        Serial.print("\tgy: "); Serial.print(gy);
        Serial.print("\tgz: "); Serial.println(gz);

        if(gx > -0.06 && gx < 0.06 && gy > -0.06 && gy < 0.06 && gz > -0.06 && gz < 0.06){
            drop_count_gyro++;
            Serial.print("drop_count_gyro: "); Serial.println(drop_count_gyro);
        }

        // State transition
        if(drop_count_pressure > 10 && drop_count_gyro > 10){
            s = State_test;
            digitalWrite(led1, HIGH);
            break;
        }
    }
}