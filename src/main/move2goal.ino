#include "cansat_define.h"
#include "cansat_prototype.h"

void move2goal(){
    unsigned long m_start = millis();
    unsigned int forward_time = 10000;
    // Update GPS value
    while(hs.available() > 0)
        gps.encode(hs.read());

    while(distance(g_lng, g_lat, gps.location.lng(), gps.location.lat()) > 5){// Until the distance from the goal is 5 meters or less,

        while(hs.available() > 0)
            gps.encode(hs.read());

        if(gps.location.isUpdated()){
            double old_direction2goal = direction(g_lng, g_lat, gps.location.lng(), gps.location.lat());
            float old_lat = gps.location.lat();
            float old_lng = gps.location.lng();

            Serial.print("Distance: ");
            Serial.print(distance(g_lng, g_lat, gps.location.lng(), gps.location.lat()));
            Serial.print("\t");
            Serial.print("Direction: ");
            Serial.print(direction(g_lng, g_lat, gps.location.lng(), gps.location.lat()));
            Serial.println();

            if(direction(g_lng, g_lat, gps.location.lng(), gps.location.lat()) < 20){
                // Reduce time to advance when below 20m
                forward_time = 5000;
            }

            m_start = millis();
            while((millis() - m_start) < forward_time){
                // Move forward if forward_time is not
                forward(255);
            }
            double tmp = old_direction2goal - direction(old_lng, old_lat, gps.location.lng(), gps.location.lat());
            if(tmp > 10){
                // Turn right
                Serial.println("\tturn right");
                digitalWrite(led1, HIGH);
                turn_right(255);
                delay(100);
                digitalWrite(led1, LOW);
            }else if(tmp < -10){
                // Turn left
                Serial.println("\tturn left");
                digitalWrite(led2, HIGH);
                turn_left(255);
                delay(100);
                digitalWrite(led2, LOW);
            }else{
                // do nothing
                Serial.println("\tdo nothing");
                digitalWrite(led1, HIGH);
                digitalWrite(led2, HIGH);
                delay(500);
                digitalWrite(led1, LOW);
                digitalWrite(led2, LOW);
            }
            
            motor_stop();
        }
    }

    motor_stop();
    s = State_exit;
    delay(20);
}

void move2goal_mag(){
    
}