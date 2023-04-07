#include <WiFi.h>
#include "esp_wpa2.h"
#include "esp_wifi.h"

const char* ssid = "Amrita-2.4G-C101"; // your ssid
// #define EAP_ID "CB.EN.U4CSE20242"
#define EAP_USERNAME "CB.EN.U4CSE20242"
#define EAP_PASSWORD ""

uint8_t counter = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  
  // WPA2 enterprise magic starts here
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_USERNAME, strlen(EAP_USERNAME));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wifi_sta_wpa2_ent_enable();
  // WPA2 enterprise magic ends here


  WiFi.begin(ssid);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      counter++;
      if(counter >= 60){
        ESP.restart();
      }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
