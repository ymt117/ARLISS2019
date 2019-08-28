#include "cansat_define.h"
#include "cansat_prototype.h"


/***************************************************************
 * Run to GOAL
 * Use [GPS]
 **************************************************************/
void run2goal(){
    double old_lat = 0.0;
    double old_lng = 0.0;
    double new_lat = 0.0;
    double new_lng = 0.0;
    double old_distance2goal = 10000.0;
    double old_direction2goal = 0.0;
    double new_distance2goal = 10000.0;
    double new_direction2goal = 0.0;
    int reached_destination_count = 0;
    int away_from_goal_count = 0;
    int forward_time = 10000; // Default is 10 seconds
    bool near_goal_flag = false;
    char buf[1024];
    String str = "";

    while(1){

        // Record your old location 10 times
        // 緯度・経度を10回所得して平均する
        for(int i=0; i<10;){
            while(hs.available() > 0)
                gps.encode(hs.read());

            if(gps.location.isUpdated()){
                old_lat += gps.location.lat();
                old_lng += gps.location.lng();
                i++;
                delay(100);
            }
        }
        old_lat = old_lat / 10;
        old_lng = old_lng / 10;

        Serial.print("old Lat: "); Serial.print(old_lat, 6);
        Serial.print("\told Lng: "); Serial.println(old_lng, 6);
        // Generate a character string to be recorded on the SD card
        // microSDカードに記録する文字列を生成する
        str += String("old Lat: "+String(old_lat, 6)+" old Lng: "+String(old_lng, 6)+"\n");
        //beep(GPS_POSITIONING);

        // Calculate the distance and direction to the goal
        // ゴールまでの距離と方位を計算する
        old_distance2goal = distance(g_lng, g_lat, old_lng, old_lat);
        old_direction2goal = direction(g_lng, g_lat, old_lng, old_lat);

        Serial.print("old Distance to GOAL: "); Serial.print(old_distance2goal);
        Serial.print("\told Direction to GOAL: "); Serial.println(old_direction2goal);
        str += String("old Distance to GOAL: "+String(old_distance2goal, 2)+" old Direction to GOAL: "+String(old_direction2goal, 2)+"\n");
        //beep(DD_CALCULATION);

        // When approaching a target point of 10 m or less
        // ゴールとの距離が10m以内のとき
        if(old_distance2goal < 10){
            s = State_goal;
            return;
        }

        // Move forward [forward_time] ms
        // [forward_time]ミリ秒だけ直進する
        go_straight(forward_time);
        delay(2000);


        for(int i=0; i<10;){
            while(hs.available() > 0)
                gps.encode(hs.read());

            if(gps.location.isUpdated()){
                new_lat += gps.location.lat();
                new_lng += gps.location.lng();
                i++;
                delay(100);
            }
        }
        new_lat = new_lat / 10;
        new_lng = new_lng / 10;

        Serial.print("new Lat: "); Serial.print(new_lat, 6);
        Serial.print("\tnew Lng: "); Serial.println(new_lng, 6);
        str += String("new Lat: "+String(new_lat, 6)+" new Lng: "+String(new_lng, 6)+"\n");
        //beep(GPS_POSITIONING);

        // Calculate the distance and direction to the goal
        // ゴールまでの距離と方位を計算する
        new_distance2goal = distance(g_lng, g_lat, new_lng, new_lat);
        new_direction2goal = direction(g_lng, g_lat, new_lng, new_lat);

        Serial.print("new Distance to GOAL: "); Serial.print(new_distance2goal);
        Serial.print("\tnew Direction to GOAL: "); Serial.println(new_direction2goal);
        str += String("new Distance to GOAL: "+String(new_distance2goal, 2)+" new Direction to GOAL: "+String(new_direction2goal, 2)+"\n");
        //beep(DD_CALCULATION);

        // Adjust the direction of the rover
        // 移動前後のゴールのある方位をもとにローバーの向きを調整する
        float diff_direction = old_direction2goal - new_direction2goal;
        if(diff_direction > 10){
            Serial.println("[!] Adjust the direction of the rover [Turn RIGHT]");
            Serial.print("direction_diff: "); Serial.println(diff_direction);
            str += String("[!] Adjust the direction of the rover [Turn RIGHT]\n"+String(diff_direction, 2)+"\n");
            turn_right(255);
            delay(100);
        }else if(diff_direction < -10){
            Serial.println("[!] Adjust the direction of the rover [Turn LEFT]");
            Serial.print("direction_diff: "); Serial.println(diff_direction);
            str += String("[!] Adjust the direction of the rover [Turn LEFT]\n"+String(diff_direction, 2)+"\n");
            turn_left(255);
            delay(100);
        }else{
            // do nothing
            Serial.print("direction_diff: "); Serial.println(diff_direction);
        }
        motor_stop();
        delay(1000);

        // When you are within 1m from the previous place
        // 以前の位置から半径2m以内にいるとき（位置が変わっていない，スタックしている）
        float diff_distance = distance(old_lng, old_lat, new_lng, new_lat);
        if(diff_distance < 2.0){
            // ignore if the goal is near
            if(near_goal_flag == true) break;

            Serial.println("[!] Judgment Stack");
            Serial.print("diff_distance: "); Serial.println(diff_distance);
            str += String("[!] Judgment Stack\ndiff_distance: "+String(diff_distance, 2)+"\n");

            // Moving back and turn
            back(255);
            delay(2000);
            turn_left(255);
            delay(500);
            motor_stop();
        }

        // When moving away from the target point
        // ゴールから遠ざかっているとき
        if((new_distance2goal - old_distance2goal) > 5){
                
            Serial.println("[!] Judgment Moving away from GOAL");
            Serial.print("diff_distance: "); Serial.println(diff_distance);
            str += String("[!] Judgment Moving away from GOAL\n");
            away_from_goal_count++;

        }else{
            Serial.print("diff_distance: "); Serial.println(diff_distance);
            away_from_goal_count = 0;
        }

        // When approaching a target point of 10 m or less
        // ゴールとの距離が10m以内のとき
        if(new_distance2goal < 10){
            s = State_goal;
            return;
        }


        // Recoding to microSD card
        // システムログをmicroSDカードに記録する
        int len = str.length();
        Serial.print("len+1: "); Serial.println(len+1);
        str.toCharArray(buf, len+1);
        writeFile("/system_log.txt", buf);

        // Initialize variable with zero
        // 変数を初期化する
        old_lat, old_lng, new_lat, new_lng = 0.0;
        str = "";
    }
}
