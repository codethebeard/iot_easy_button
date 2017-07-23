#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

int TRIGGER_PIN = D8; // D0 Button.

void setup() {
    Serial.begin(115200);

    // Setup Wifi
    WiFiManager wifiManager;
    // wifiManager.resetSettings(); //For Debugging Purposes.
    wifiManager.autoConnect("ITEasy Button", "thatwaseasy");

    pinMode(TRIGGER_PIN, INPUT);
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  if ( digitalRead(TRIGGER_PIN) == HIGH ) {
    Serial.println("click");
    digitalWrite(BUILTIN_LED, LOW);

    WiFiManager wifiManager;

    if (!wifiManager.startConfigPortal("ITEasy Button", "thatwaseasy")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      ESP.reset();
      delay(5000);
    }
  }

}
