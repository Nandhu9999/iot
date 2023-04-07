#define BTNPIN 4

int btn;
int bflag = 0;
int bval = 0;
void setup() {
  Serial.begin(115200);delay(200);
  Serial.flush();Serial.println("Setup complete!");
  // esp32 start sequence...
  pinMode(2, OUTPUT);digitalWrite(2,HIGH);delay(150);digitalWrite(2,LOW);delay(150);digitalWrite(2,HIGH);delay(150);digitalWrite(2,LOW);
  
  pinMode(BTNPIN, INPUT_PULLUP);
}

void loop() {
  btn = digitalRead(BTNPIN);

  if(btn == 0 && bflag == 0){bflag = 1;bval += 1;}
  if(btn == 1){bflag = 0;}

  if(bflag == 1){
    if(bval % 2 == 0){
      digitalWrite(2,HIGH);
      Serial.println("TOGGLE ON");
    }else{
      digitalWrite(2,LOW);
      Serial.println("TOGGLE OFF");
    }
  }
}
