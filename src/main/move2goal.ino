#include "cansat_define.h"
#include "cansat_prototype.h"

/***************************************************************
 * Run to GOAL
 * Use [GPS]
 **************************************************************/
void move2goal(){

    timer_start_3();
    unsigned long m_start = millis();
    unsigned int forward_time = 5000;
    Serial.println("move2goal");
    // Update GPS value
    while(hs.available() > 0)
        gps.encode(hs.read());

    while(distance(g_lng, g_lat, gps.location.lng(), gps.location.lat()) > 5){// Until the distance from the goal is 5 meters or less,

        while(hs.available() > 0)
            gps.encode(hs.read());

        if(gps.location.isUpdated()){
            writeAll();

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
                forward_time = 2500;
            }

            Serial.println("\tforward");
            m_start = millis();
            while((millis() - m_start) < forward_time){
                // Move forward if forward_time is not
                
                //forward(255);
                right_cw(150);
                left_ccw(220);
            }
            motor_stop();
            delay(2000);

            double tmp = old_direction2goal - direction(old_lng, old_lat, gps.location.lng(), gps.location.lat());
            if(tmp > 10){
                // Turn right
                Serial.println("\tturn right");
                beep(PUSHED);
                digitalWrite(led1, HIGH);
                turn_right(255);
                delay(100);
                digitalWrite(led1, LOW);
            }else if(tmp < -10){
                // Turn left
                beep(PUSHED);
                beep(PUSHED);
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
            delay(2000);
        }
    }

    motor_stop();
    s = State_exit;
    delay(20);
}


/***************************************************************
 * Run to GOAL
 * Use [GPS] and [MAG]
 **************************************************************/
void move2goal_mag(){
    // Timer start: imu_init function is called every 20ms
    timer_start_3();

    // Update GPS value
    while(hs.available() > 0)
        gps.encode(hs.read());

    do{
        if(gps.location.isUpdated()){
            writeAll();

            double goal_distance = distance(g_lng, g_lat, gps.location.lng(), gps.location.lat());
            double goal_direction = direction(g_lng, g_lat, gps.location.lng(), gps.location.lat());

            motor_stop();
            delay(1000);
            // Calculate the difference between the direction of the goal and the direction CanSat
            double angle_difference = goal_direction - ((ToDeg(MAG_Heading)) + 180);
            if(angle_difference > -60 && angle_difference < 60){// +-60
                digitalWrite(led1, HIGH);
                digitalWrite(led2, HIGH);
                forward(200);
                delay(10000);
                motor_stop();
                digitalWrite(led1, LOW);
                digitalWrite(led2, LOW);
            }else if(angle_difference >= 60){// Over +60
                digitalWrite(led1, HIGH);
                if(angle_difference > 180){// Over +180
                    turn_left(255);
                    delay(100);
                    motor_stop();
                    delay(500);
                }else{// Under +180
                    turn_right(255);
                    delay(100);
                    motor_stop();
                    delay(500);
                }
                digitalWrite(led1, LOW);
            }else if(angle_difference <= -60){// Under -60
                digitalWrite(led2, HIGH);
                if(angle_difference > -180){// Over -180
                    turn_left(255);
                    delay(100);
                    motor_stop();
                    delay(500);
                }else{// Under -180
                    turn_right(255);
                    delay(100);
                    motor_stop();
                    delay(500);
                }
                digitalWrite(led2, LOW);
            }

            // Judge rollover
            motor_stop();
            delay(1000);
            if(ToDeg(roll) > 70 || ToDeg(roll) < -70){// roll value > 70 or roll value < -70
                beep(PUSHED);
                turn_right(255);
                delay(10000);
                turn_left(255);
                delay(10000);
                motor_stop();
            }
        }
    }while(distance(g_lng, g_lat, gps.location.lng(), gps.location.lat()) > 10);// Until the distance from the goal is 10 meters or less,
    
    s = State_goal;
}