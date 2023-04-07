
void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  delay(100);
  Serial.println("setup completed..");
}

void loop() {
  Serial.println("Waiting for value..");
  while (Serial.available() == 0) {}

  int input = Serial.parseInt();
  Serial.print(">> ");Serial.println(input);
  if(input == 1){
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
  }
  Serial.println("\n\n\n\n\n\n\n\n\n\n\n");
}
