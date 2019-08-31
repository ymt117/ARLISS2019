#include "cansat_define.h"
#include "cansat_prototype.h"


/***************************************************************
 * Run to GOAL
 * Use [GPS]
 **************************************************************/
void run2goal(){
    double old_lat = 0.0;               // 移動する前のローバの緯度
    double old_lng = 0.0;               // 移動する前のローバの経度
    double new_lat = 0.0;               // 移動した後のローバの緯度
    double new_lng = 0.0;               // 移動した後のローバの経度
    double old_distance2goal = 10000.0; // 移動する前のローバとゴールの距離
    double new_distance2goal = 10000.0; // 移動した後のローバとゴールの距離
    double direction_theta = 0.0;       // 移動した後のローバから見て，ゴールのある方角(真北を0°)
    double direction_phi = 0.0;         // 移動した後のローバから見て，移動する前のローバのある方角(真北を0°)
    int turn_angle = 0;                 // 旋回時の旋回角度
    int turn_time = 8000;               // 旋回時のモータの制御量(delayの時間[ms])
    int forward_time = 15000;           // 直進時のモータの制御量(delayの時間[ms])
    double diff_direction = 0.0;        // 直進の向き修正角度

    char buf[1024];
    String str = "";

    while(1){

        delay(2000);

        // Record your old location 10 times
        // 緯度・経度を10回所得して平均する
        old_lat = 0.0;
        old_lng = 0.0;
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

        writeAll();

#ifdef CANSAT_SERIAL_DEBUG
        Serial.print("old Lat: "); Serial.print(old_lat, 6);
        Serial.print("\told Lng: "); Serial.println(old_lng, 6);
#endif
        // Generate a character string to be recorded on the SD card
        // microSDカードに記録する文字列を生成する.
        str += String("old Lat: "+String(old_lat, 6)+"\nold Lng: "+String(old_lng, 6)+"\n");
        //beep(GPS_POSITIONING);

        // Calculate the distance to the goal
        // ゴールまでの距離を計算する
        old_distance2goal = 0.0;
        old_distance2goal = distance(g_lng, g_lat, old_lng, old_lat);
        distance2goal = old_distance2goal;

#ifdef CANSAT_SERIAL_DEBUG
        Serial.print("old Distance to GOAL: "); Serial.println(old_distance2goal);
#endif
        str += String("old Distance to GOAL: "+String(old_distance2goal, 2)+"\n");
        //beep(DD_CALCULATION);

        // When approaching a target point of 10 m or less
        // ゴールとの距離が10m以内のとき
        if(old_distance2goal < 10){
            s = State_goal;
            return;
        }

        // Move forward [forward_time] ms
        // [forward_time]ミリ秒だけ直進する
        Serial.print("Moving forward "); Serial.print(forward_time); Serial.println(" [ms]");
        str += String("Moving forward "+String(forward_time)+" [ms]\n");
        diff_direction = 0.0;
        go_straight(forward_time, 0);
        Serial.println("Motor stop");
        str += "Motor stop\n";
        delay(2000);

        new_lat = 0.0;
        new_lng = 0.0;
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

        writeAll();

#ifdef CANSAT_SERIAL_DEBUG
        Serial.print("new Lat: "); Serial.print(new_lat, 6);
        Serial.print("\tnew Lng: "); Serial.println(new_lng, 6);
#endif
        str += String("new Lat: "+String(new_lat, 6)+"\nnew Lng: "+String(new_lng, 6)+"\n");
        //beep(GPS_POSITIONING);

        // Calculate the distance to the goal
        // ゴールまでの距離を計算する
        new_distance2goal = 0.0;
        new_distance2goal = distance(g_lng, g_lat, new_lng, new_lat);
        // 移動した後のローバから見て，ゴールのある方角を計算する
        direction_theta = direction(new_lng, new_lat, g_lng, g_lat);
        // 移動した後のローバから見て，移動する前のローバのある方角を計算する
        direction_phi = direction(new_lng, new_lat, old_lng, old_lat);

#ifdef CANSAT_SERIAL_DEBUG
        Serial.print("new Distance to GOAL: "); Serial.println(new_distance2goal);
        Serial.print("direction_theta: "); Serial.println(direction_theta);
        Serial.print("direction_phi: "); Serial.println(direction_phi);
#endif
        str += String("new Distance to GOAL: "+String(new_distance2goal, 2)+"\n");
        str += String("direction_tehta: "+String(direction_theta, 2)+"\n");
        str += String("direction_phi: "+String(direction_phi, 2)+"\n");
        //beep(DD_CALCULATION);

        // Adjust the direction of the rover
        // ローバの向きを修正する
        diff_direction = 0.0;
        diff_direction = direction_theta - direction_phi;
        #ifdef CANSAT_SERIAL_DEBUG
        Serial.print("diff_direction: "); Serial.println(diff_direction);
        #endif
        str += String("diff_direction: "+String(diff_direction, 2)+"\n");
        if(diff_direction >= -360 && diff_direction <= -180){
            turn_angle = (int)(-180-diff_direction);

            #ifdef CANSAT_SERIAL_DEBUG
            Serial.print("[!] Adjust the direction of the rover [Turn LEFT] ");
            Serial.print(turn_angle); Serial.println(" [ms]");
            #endif
            str += String("[!] Adjust the direction of the rover [Turn LEFT] "+String(turn_angle)+" [ms]\n");
        
            go_straight(turn_time, turn_angle);

        }else if(diff_direction > -180 && diff_direction < 0){
            turn_angle = (int)(+180+diff_direction);

            #ifdef CANSAT_SERIAL_DEBUG
            Serial.println("[!] Adjust the direction of the rover [Turn RIGHT] ");
            Serial.print(turn_angle); Serial.println(" [ms]");
            #endif
            str += String("[!] Adjust the direction of the rover [Turn RIGHT] "+String(turn_angle)+" [ms]\n");
            
            go_straight(turn_time, -turn_angle);

        }else if(diff_direction >= 0 && diff_direction <= 180){
            turn_angle = (int)(+180-diff_direction);

            #ifdef CANSAT_SERIAL_DEBUG
            Serial.println("[!] Adjust the direction of the rover [Turn LEFT] ");
            Serial.print(turn_angle); Serial.println(" [ms]");
            #endif
            str += String("[!] Adjust the direction of the rover [Turn LEFT] "+String(turn_angle)+" [ms]\n");
            
            go_straight(turn_time, turn_angle);

        }else if(diff_direction > 180 && diff_direction <= 360){
            turn_angle = (int)(-180+diff_direction);

            #ifdef CANSAT_SERIAL_DEBUG
            Serial.println("[!] Adjust the direction of the rover [Turn RIGHT] ");
            Serial.print(turn_angle); Serial.println(" [ms]");
            #endif
            str += String("[!] Adjust the direction of the rover [Turn RIGHT]"+String(turn_angle)+" [ms]\n");
            
            go_straight(turn_time, -turn_angle);

        }
        else{
            // do nothing
            Serial.println("do nothing turn");
        }
        motor_stop();
        delay(1000);

        // When you are within 1m from the previous place
        // 以前の位置から半径3m以内にいるとき（位置が変わっていない，スタックしている）
        float diff_distance = distance(old_lng, old_lat, new_lng, new_lat);
        if(diff_distance < 3.0){
#ifdef CANSAT_SERIAL_DEBUG
            twe_lite_wakeup();
            Serial.println("[!] Judgment Stack");
            Serial.print("diff_distance: "); Serial.println(diff_distance);
            twe_lite_sleep();
#endif
            str += String("[!] Judgment Stack\ndiff_distance: "+String(diff_distance, 2)+"\n");

            // Moving back and turn
            back(255);
            delay(3000);
            turn_left(255);
            delay(500);
            motor_stop();
        }
        else{
            Serial.print("NO Stack diff_distance: "); Serial.println(diff_distance);
        }

        // When moving away from the target point
        // ゴールから遠ざかっているとき
        if((new_distance2goal - old_distance2goal) > 5){
#ifdef CANSAT_SERIAL_DEBUG
            Serial.println("[!] Judgment Moving away from GOAL");
#endif
            str += String("[!] Judgment Moving away from GOAL\n");

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
        str.toCharArray(buf, len+1);
        writeFile("/system_log.txt", buf);

        // Initialize variable with zero
        // 変数を初期化する
        old_lat, old_lng, new_lat, new_lng = 0.0;
        str = "";
    }
}
