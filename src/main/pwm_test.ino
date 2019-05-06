static const int pwm1 = PB0;
static const int pwm2 = PA7;

void pwm_init(){
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
}

void pwm_test(){
  for(int i=0; i<255; i++){
    analogWrite(pwm1, i);
    analogWrite(pwm2, i);
    delay(20);
  }
}
