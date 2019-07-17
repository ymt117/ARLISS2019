#define MY_TIMER TIM4

// Handler for stimer
static stimer_t TimHandle;

volatile int my_timer_counter = 0;

static void _call_func(stimer_t *htim){
  UNUSED(htim);
  //Serial.println(my_timer_counter++);
  //imu_test();
}

void timer_init(){
  // Set TIMx instance.
  TimHandle.timer = MY_TIMER;
  // Timer set to 10ms=10000
  TimerHandleInit(&TimHandle, 10000 - 1, ((uint32_t)(getTimerClkFreq(MY_TIMER) / (1000000)) - 1));
  attachIntHandle(&TimHandle, _call_func);
}

void imu_test(){
  if((millis()-timer)>=20)  // Main loop runs at 50Hz
  {
    counter++;
    timer_old = timer;
    timer=millis();
    if (timer>timer_old)
    {
      G_Dt = (timer-timer_old)/1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
      if (G_Dt > 0.2)
        G_Dt = 0; // ignore integration times over 200 ms
    }
    else
      G_Dt = 0;
      

    // *** DCM algorithm
    // Data adquisition
    Read_Gyro();   // This read gyro data
    Read_Accel();     // Read I2C accelerometer

    if (counter > 5)  // Read compass data at 10Hz... (5 loop runs)
    {
      counter=0;
      Read_Compass();    // Read I2C magnetometer
      Compass_Heading(); // Calculate magnetic heading
    }

    // Calculations...
    Matrix_update();
    Normalize();
    Drift_correction();
    Euler_angles();
    // ***

    
    //Serial.print("ANG:");
    Serial.print(ToDeg(roll));
    Serial.print("\t");
    Serial.print(ToDeg(pitch));
    Serial.print("\t");
    Serial.print(ToDeg(yaw));
    Serial.println();
    

    //printdata();
  }
}
