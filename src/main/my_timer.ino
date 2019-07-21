#define MY_TIMER TIM4

// Handler for stimer
static stimer_t TimHandle;

volatile int my_timer_counter = 0;

static void _call_func(stimer_t *htim){
  UNUSED(htim);
  //madgwick_update();
}

void timer_init(){
  // Set TIMx instance.
  TimHandle.timer = MY_TIMER;
  // Timer set to 10ms=10000
  TimerHandleInit(&TimHandle, 50000 - 1, ((uint32_t)(getTimerClkFreq(MY_TIMER) / (1000000)) - 1));
  attachIntHandle(&TimHandle, _call_func);
}
