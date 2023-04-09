// https://randomnerdtutorials.com/esp32-web-server-arduino-ide/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
Adafruit_BMP280 bme; // I2C

#include <WiFi.h>

const char* ssid = "OnePlus 8 Pro";
const char* password = "oneplusequals";

WiFiServer server(80);
String header; // Variable to store the HTTP request
String webpage;
unsigned long currentTime = millis(); // Current time
unsigned long previousTime = 0; // Previous time
const long timeoutTime = 2000; // Define timeout time in milliseconds (example: 2000ms = 2s)

void setup() {
  Serial.begin(115200);
  bme.begin(0x76);

  WiFi.disconnect(true);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Print local IP address and start web server
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

float deg, pa, m;
void loop() {
  deg = bme.readTemperature();
  pa = bme.readPressure();
  m = bme.readAltitude(1013.25);
  //Serial.printf("temperature=%f &pressure=%f &altitude=%f", deg, pa, m);Serial.println();
  delay(10);

  WiFiClient client = server.available();
  if (client) { 
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client Connected.\n");
    String currentLine = "";
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            if (header.indexOf("GET /data") >= 0) { webpage = "data"; }
            else if (header.indexOf("GET /templatePage") >= 0) { webpage = "templatePage"; }

            if ( webpage == "data" ){
              client.println("HTTP/1.1 200 OK");
              client.println("Cache-Control: no-cache, no-store, must-revalidate");
              client.println("Content-type:application/json");
              client.println("Connection: close");
              client.println();
              client.println("[" + String(deg) + "," + String(pa) + "," + String(m) + "]");
            }
            else if( webpage == "templatePage" || webpage == "" ){
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
          
              client.println("<!DOCTYPE html><html>");
              client.println("<body><h1>ESP32 Web Server</h1>");
              client.println("<div id='content'>___</div>");
              client.println("<script>setInterval( async()=>{ const response = await fetch('/data'); const mydata = await response.json(); update(mydata) }, 500 );");
              client.println("function update(data){ document.querySelector(\"#content\").innerHTML='temperature ='+data[0]+'<br>pressure='+data[1]+'<br>altitude='+data[2]; }</script>");
              client.println("</body></html>");
              client.println();
            } 

            break; // Break out of the while loop
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";

    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
