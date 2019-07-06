#define MY_TIMER_EXIT TIM4

// Handler for stimer
static stimer_t TimHandle;

static void _sw_counter_reset(stimer_t *htim){
  UNUSED(htim);
  sw_counter = 0;
  digitalWrite(led2, !digitalRead(led2));
}

void timer_init(){
  // Set TIMx instance.
  TimHandle.timer = MY_TIMER_EXIT;
  // Timer set to 10ms
  TimerHandleInit(&TimHandle, 100000 - 1, ((uint32_t)(getTimerClkFreq(MY_TIMER_EXIT) / (1000000)) - 1));
  attachIntHandle(&TimHandle, _sw_counter_reset);
}
