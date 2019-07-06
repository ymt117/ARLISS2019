#define MY_TIMER_EXIT TIM4

// Handler for stimer
static stimer_t TimHandle;

volatile uint8_t press_old_state = 0;
volatile uint16_t press_time = 0;

static void _exit(stimer_t *htim){
  UNUSED(htim);
  // Count while button is pressed.
  if(!digitalRead(sw) && press_old_state == 1){
    press_time++;
    //digitalWrite(led2, !digitalRead(led2));
    //Serial.println(press_time);
    if(press_time > 50) s = State_exit;
  }
  else if(!digitalRead(sw)){
    press_old_state = 1;
  }
  // Reset count if button is not pressed.
  else{
    press_old_state = 0;
    press_time = 0;
  }
}

void timer_init(){
  // Set TIMx instance.
  TimHandle.timer = MY_TIMER_EXIT;
  // Timer set to 10ms=100000?
  TimerHandleInit(&TimHandle, 100000 - 1, ((uint32_t)(getTimerClkFreq(MY_TIMER_EXIT) / (1000000)) - 1));
  attachIntHandle(&TimHandle, _exit);
}
