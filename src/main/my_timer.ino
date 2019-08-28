#include "cansat_define.h"
#include "cansat_prototype.h"

#define MY_TIMER_3  TIM3
#define MY_TIMER_4  TIM4
#define MY_TIMER_12 TIM12

// Handler for stimer
static stimer_t TimHandle_3;
static stimer_t TimHandle_4;
static stimer_t TimHandle_12;

volatile int my_timer_counter = 0;
volatile long old_millis_3 = 0;
volatile long old_millis_4 = 0;
volatile long old_micros_12 = 0;

/***************************************************************
 * Function called by timer interrupt
 **************************************************************/
static void _call_func_3(stimer_t *htim){
  UNUSED(htim);
  //imu_ahrs_update();
  //madgwick_update();
}

static void _call_func_4(stimer_t *htim){
  UNUSED(htim);
  Serial.print("Timer 4 called: "); Serial.println(millis() - old_millis_4);
  old_millis_4 = millis();
}

static void _call_func_12(stimer_t *htim){
  UNUSED(htim);
  madgwick_update();
  //Serial.print("TIM12 called: "); Serial.println(micros() - old_micros_12);
  //old_micros_12 = micros();
}

/***************************************************************
 * TIM 3 Initialization
 **************************************************************/
void timer_start_3(){
  // Set TIMx instance.
  TimHandle_3.timer = MY_TIMER_3;
  // Timer set to 10ms=10000
  TimerHandleInit(&TimHandle_3, 20000 - 1, ((uint32_t)(getTimerClkFreq(MY_TIMER_3) / (1000000)) - 1));
  attachIntHandle(&TimHandle_3, _call_func_3);

  Serial.println("[!] Tim3 start");
}

void timer_stop_3(){
  TimHandle_3.timer = MY_TIMER_3;
  TimerHandleDeinit(&TimHandle_3);

  Serial.println("[!] Tim3 stop");
}

/***************************************************************
 * TIM 4 Initialization
 **************************************************************/
void timer_start_4(){
  // Set TIMx instance.
  TimHandle_4.timer = MY_TIMER_4;
  // Timer set to 10ms=10000
  TimerHandleInit(&TimHandle_4, 50000 - 1, ((uint32_t)(getTimerClkFreq(MY_TIMER_4) / (1000000)) - 1));
  attachIntHandle(&TimHandle_4, _call_func_4);
}

/***************************************************************
 * TIM 12 Initialization
 **************************************************************/
void timer_start_12(){
  // Set TIMx instance.
  TimHandle_12.timer = MY_TIMER_12;
  // Timer set to 10ms=10000
  TimerHandleInit(&TimHandle_12, 20000 - 1, ((uint32_t)(getTimerClkFreq(MY_TIMER_12) / (1000000)) - 1));
  attachIntHandle(&TimHandle_12, _call_func_12);

  Serial.println("[!] Tim12 start");
}

void timer_stop_12(){
  TimHandle_12.timer = MY_TIMER_12;
  TimerHandleDeinit(&TimHandle_12);

  Serial.println("[!] Tim12 stop");
}