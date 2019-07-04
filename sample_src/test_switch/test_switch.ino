static const int sw = PC13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sw, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead(sw)){
    Serial.println("Pushed!");
    delay(100);
  }
  
}
