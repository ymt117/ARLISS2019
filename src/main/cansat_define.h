#ifndef __CANSAT_DEFINE__
#define __CANSAT_DEFINE__

/***************************************************************
 * GOAL Position
 **************************************************************/
static const float g_lng = 130.543375;
static const float g_lat = 31.567613;

/***************************************************************
 * Status of CanSat
 **************************************************************/
enum MyState{
  State_init = 0,
  State_launch_detect,
  State_release_detect,
  State_drop_detect,
  State_first_fire,
  State_second_fire,
  State_run_to_goal,
  State_goal,
  State_test,
  State_exit
};
MyState s = State_init;

/***************************************************************
 * Speaker
 **************************************************************/
#define BOOT_UP          1
#define SHUT_DOWN        2
#define CLANNAD          3
#define CANSAT_ERROR     4
#define PUSHED           5
#define CANSAT_ERROR2    6
#define LAUNCH_COMPLETE  7
#define RELEASE_COMPLETE 8
#define DROP_COMPLETE    9

static const uint8_t sp = PB12; // Pin connected to the Speaker

/***************************************************************
 * TWE-Lite
 **************************************************************/
static const uint8_t twe_lite_sleep_pin = PA15; // Pin connected to the TWE-Lite M3(LOW: Sleep)

/***************************************************************
 * LED
 **************************************************************/
static const uint8_t led1 = PB13; // Pin connected to the LED1
static const uint8_t led2 = PB14; // Pin connected to the LED2

/***************************************************************
 * CdS
 **************************************************************/
static const uint8_t cds = PA0; // Pin connected to the CdS

/***************************************************************
 * Switch
 **************************************************************/
static const uint8_t sw = PC13; // Pin connected to the Switch
volatile uint16_t press_counter = 0;

/***************************************************************
 * Battery
 **************************************************************/
static const uint8_t batt1 = PA4; // Pin to monitor battery voltage(Microcomputer and so on)
static const uint8_t batt2 = PB0; // Pin to monitor battery voltage(Motor and Heating wire)

/***************************************************************
 * Motor
 **************************************************************/
static const uint8_t ain1 = PC7; // Pin connected to the AIN1
static const uint8_t ain2 = PC8; // Pin connected to the AIN2
static const uint8_t pwma = PB5; // Pin connected to the PWMA

static const uint8_t bin1 = PB4; // Pin connected to the BIN1
static const uint8_t bin2 = PB10; // Pin connected to the BIN2
static const uint8_t pwmb = PA8; // Pin connected to the PWMB

/***************************************************************
 * Heating Pin
 **************************************************************/
 static const uint8_t heat1 = PA11; // Pin connected to the Heat1
 static const uint8_t heat2 = PA12; // Pin connected to the Heat2


/***************************************************************
 * IMU
 **************************************************************/
// IMU Initialize failed: -1, successful: 1
int imu_status = -1;

float altitude_offset_value = 0.0;
 
// Uncomment the below line to use this axis definition:
   // X axis pointing forward
   // Y axis pointing to the right
   // and Z axis pointing down.
// Positive pitch : nose up
// Positive roll : right wing down
// Positive yaw : clockwise
int SENSOR_SIGN[9] = {1,1,1,-1,-1,-1,1,1,1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer

// accelerometer: 8 g sensitivity
// 3.9 mg/digit; 1 g = 256
#define GRAVITY 2048  //this equivalent to 1G in the raw data coming from the accelerometer


#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi

// gyro: 2000 dps full scale
// 70 mdps/digit; 1 dps = 0.07
#define Gyro_Gain_X 0.07 //X axis Gyro gain
#define Gyro_Gain_Y 0.07 //Y axis Gyro gain
#define Gyro_Gain_Z 0.07 //Z axis Gyro gain
#define Gyro_Scaled_X(x) ((x)*ToRad(Gyro_Gain_X)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Y(x) ((x)*ToRad(Gyro_Gain_Y)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Z(x) ((x)*ToRad(Gyro_Gain_Z)) //Return the scaled ADC raw data of the gyro in radians for second

#define M_X_MIN -1000
#define M_Y_MIN -1000
#define M_Z_MIN -1000
#define M_X_MAX +1000
#define M_Y_MAX +1000
#define M_Z_MAX +1000

#define Kp_ROLLPITCH 0.02
#define Ki_ROLLPITCH 0.00002
#define Kp_YAW 1.2
#define Ki_YAW 0.00002

/*For debugging purposes*/
//OUTPUTMODE=1 will print the corrected data,
//OUTPUTMODE=0 will print uncorrected data of the gyros (with drift)
#define OUTPUTMODE 1

#define PRINT_DCM 0     //Will print the whole direction cosine matrix
#define PRINT_ANALOGS 0 //Will print the analog raw data
#define PRINT_EULER 1   //Will print the Euler angles Roll, Pitch and Yaw

volatile float G_Dt=0.02;    // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible
//volatile float G_Dt=0.1;

long timer=0;   //general purpuse timer
long timer_old;
long timer24=0; //Second timer used to print
int AN[6]; //array that stores the gyro and accelerometer data
int AN_OFFSET[6]={0,0,0,0,0,0}; //Array that stores the Offset of the sensors
int MAG_OFFSET[9] = {14041,-1800,2100,-6200,12000,4400,6120,-4100,8200}; // x_max,x_min,y_max,y_min,z_max,z_min

float gyro_x;
float gyro_y;
float gyro_z;
float accel_x;
float accel_y;
float accel_z;
float magnetom_x;
float magnetom_y;
float magnetom_z;
double c_magnetom_x;
double c_magnetom_y;
double c_magnetom_z;
double MAG_Heading;

float Accel_Vector[3]= {0,0,0}; //Store the acceleration in a vector
float Gyro_Vector[3]= {0,0,0};//Store the gyros turn rate in a vector
float Omega_Vector[3]= {0,0,0}; //Corrected Gyro_Vector data
float Omega_P[3]= {0,0,0};//Omega Proportional correction
float Omega_I[3]= {0,0,0};//Omega Integrator
float Omega[3]= {0,0,0};

// Euler angles
float roll;
float pitch;
float yaw;

float errorRollPitch[3]= {0,0,0};
float errorYaw[3]= {0,0,0};

volatile unsigned int counter=0;
byte gyro_sat=0;

float DCM_Matrix[3][3]= {
  {
    1,0,0  }
  ,{
    0,1,0  }
  ,{
    0,0,1  }
};
float Update_Matrix[3][3]={{0,1,2},{3,4,5},{6,7,8}}; //Gyros here


float Temporary_Matrix[3][3]={
  {
    0,0,0  }
  ,{
    0,0,0  }
  ,{
    0,0,0  }
};

#endif