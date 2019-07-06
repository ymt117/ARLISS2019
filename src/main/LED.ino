// LED

void led_init(){
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void blink(){
  digitalWrite(led1, HIGH);
  digitalWrite(led2, LOW);
  delay(500);
  digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  delay(500);
}

void led1_blink_fast(){
  digitalWrite(led1, HIGH);
  delay(50);
  digitalWrite(led1, LOW);
  delay(50);
}

void led_off(){
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
}
