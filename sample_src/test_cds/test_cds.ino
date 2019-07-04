static const int cds = PA0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello");
  pinMode(cds, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(cds));
  delay(10);
}
