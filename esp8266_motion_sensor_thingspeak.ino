//  ESP8266 Motion Sensor ArduinoIDE Thingspeak IoT Example code
//  https://vaasa.hacklab.fi
//
//  This sketch sends data via HTTP GET requests to api.thingspeak.com when 
//  motion is detected on external motion detector
// 
//  The board used is ESP8266 development board - Wittyboard. You need an api_key from Thingspeak.
//  The relay connection from motion sensor is connected to the button pin for easy testing.
//  Test the thing without motion sensor by pushing the button.
// 
//  ESP8266 Witty: http://www.schatenseite.de/en/2016/04/22/esp8266-witty-cloud-module/
//
//  Based on: https://vaasa.hacklab.fi/2016/02/06/esp8266-on-nodemcu-board-ds18b20-arduinoide-thingspeak/
//  Based on: https://codegists.com/snippet/arduino/twitwireino_jasonpellerin_arduino
//

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "VaasaHacklab";
const char* pass = "wifikey";

// RGB or LDR pins are not used in this example
#define RED_PIN      7   // RGB Light pin at Arduino pin 3
#define GREEN_PIN    8   // RGB Light pin at Arduino pin 5
#define BLUE_PIN     9   // RGB Light pin at Arduino pin 6
#define BUTTON_PIN   4   // Alert When HIGH button
#define LDR_PIN      A0

char host[] = "api.thingspeak.com";
String ApiKey = "THINGSPEAK_API_KEY";
String path = "/update?key=" + ApiKey + "&field1=";

int val = 0;
int old_val = 0;
int state = 0;

void setup() {

  Serial.begin(115200);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, 0);

}

void loop() {
  val = digitalRead(BUTTON_PIN);
  // check if there was a state transition
  if ((val == HIGH) && (old_val == LOW)){
    state = 1 - state;
    delay(10);
  }
  old_val = val; // val is now old, let's store it

  if (state == 1) {  // Sensor has changed the state, time to make a GET request to the ThingSpeak API,
    alert();
    Serial.print("connecting to ");
    Serial.println(host);
    Serial.println("thingspeak connect");
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return;
    }
    client.print(String("GET ") + path + val + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: keep-alive\r\n\r\n");
    delay(500);

    Serial.println("closing connection");
    state = 0;
  }
}

void alert() {
   Serial.println("Intruder Alert!!!");
}
