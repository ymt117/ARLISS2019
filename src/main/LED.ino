// LED

void led_init(){
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void led(int led, int led_status){
  if(!(led == led1 | led == led2)){
    Serial.println("Error! First argument is not good!");
    return;
  }
  
  if(led_status == ON){
    digitalWrite(led, HIGH);
  }
  else if(led_status == OFF){
    digitalWrite(led, LOW);
  }
  else{
    Serial.println("Error! Second argument is not good!");
  }
}
