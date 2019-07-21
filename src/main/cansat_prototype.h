#ifndef __CANSAT_PROTOTYPE__
#define __CANSAT_PROTOTYPE__

/***************************************************************
 * Speaker: speaker.ino
 **************************************************************/
void speaker_init();
void _beep(float *, int, int);
void beep(int);

/***************************************************************
 * LED: LED.ino
 **************************************************************/
void led_init();
void blink();
void led1_blink_fast();
void led_off();

/***************************************************************
 * Motor: motor.ino
 **************************************************************/
void motor_init();
void left_cw(int);
void left_ccw(int);
void right_cw(int);
void right_ccw(int);
void forward(int);
void back(int);
void turn_left(int);
void turn_right(int);
void motor_stop();
void rollover_return();

/***************************************************************
 * IMU: 
 **************************************************************/
// imu.ino
void imu_init();
void imu_offset();
void imu_plot();
void Read_Gyro();
void Read_Accel();
void Read_Compass();
float calc_g_force(float, float, float);
void compass_calibrate();
void imu_test();

// DCM.ino
void Normalize(void);
void Drift_correction(void);
void Matrix_update(void);
void Euler_angles(void);

// matrix.ino
void Matrix_Multiply(float [][3], float [][3], float [][3]);

// vector.ino
float Vector_Dot_Product(float *, float *);
void Vector_Cross_Product(float *, float *, float *);
void Vector_Scale(float *, float *, float);
void Vector_Add(float *, float *, float *);

// madgwick.ino
void madgwick_init();
void madgwick_update();

// my_math.ino
float _min(float, float);
float _max(float, float);

/***************************************************************
 * Timer: my_timer.ino
 **************************************************************/
static void _call_func(stimer_t *);
void timer_init();

/***************************************************************
 * GPS: gps.ino
 **************************************************************/
void gps_init();
void gps_read();
void gps_plain_text();

/***************************************************************
 * SD Card sd.ino
 **************************************************************/
void sd_init();
void writeFile(const char*, const char*);

/***************************************************************
 * Test Code: test.ino
 **************************************************************/
void static_laod();
void printdata();
void turn_north();
void heat_test();
void test_run();

#endif