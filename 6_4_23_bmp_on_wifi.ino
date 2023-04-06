#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#include <HTTPClient.h>

#include <WiFi.h>
#include "esp_wpa2.h"
#include "esp_wifi.h"

const char* ssid = "Amrita-2.4G-C101";
#define EAP_USERNAME "CB.EN.U4CSE20242"
#define EAP_PASSWORD ""

String serverName = "https://amrita-iot.glitch.me";

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

Adafruit_BMP280 bme; // I2C
//Adafruit_BMP280 bme(BMP_CS); // hardware SPI
//Adafruit_BMP280 bme(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);
 
uint8_t counter = 0;

void setup() {
  Serial.begin(115200);
  
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }

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
  float c, pa, m;
  c = bme.readTemperature();
  pa = bme.readPressure();
  m = bme.readAltitude(1013.25);
  // Serial.printf("temperature=%f &pressure=%f &altitude=%f", c, pa, m);Serial.println();
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      String serverPath = serverName + "/bmp?temperature=" + String(c) + "&pressure=" + String(pa) + "&altitude=" + String(m);
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
      http.end();
  }
  delay(2500);
}
