#include <WiFi.h>

// uint8_t customMacAddress[] = {0x24, 0x0a, 0xc4, 0x9a, 0xa5, 0xa1}

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  // esp_wifi_set_mac(ESP_IF_WIFI_STA, &customMacAddress[0]) // i can use custom mac adddress if necessary
  Serial.print("MAC >> ");
  Serial.println(WiFi.macAddress());
}

void loop() {
}
