#define POTENTIOMETER_PIN 14

int val = 0;
void setup() {
  Serial.begin(115200);
  pinMode(POTENTIOMETER_PIN, OUTPUT);
}

void loop() {
  val = analogRead(POTENTIOMETER_PIN);
  Serial.println(val);
  delay(100);
}
