#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "OnePlus 8 Pro";
const char* password = "";

String serverName = "https://iot-amrita.glitch.me/";

void setup() {
  Serial.begin(115200); 
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nCONNECTED TO WIFI!");
  pinMode(2,OUTPUT);
}

void loop() {
  digitalWrite(2,HIGH);
  delay(100);
  digitalWrite(2,LOW);
  delay(100);

  if(WiFi.status()== WL_CONNECTED){
    if(true){ // GET REQUEST
      HTTPClient http;
      String serverPath = serverName + "/";
      http.begin(serverPath.c_str());
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }

  }
}
