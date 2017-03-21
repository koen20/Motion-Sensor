/*
 * PIR sensor tester
 */
 #include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <stdlib.h>

ADC_MODE(ADC_VCC)

#include <ESP8266HTTPClient.h>
ESP8266WiFiMulti WiFiMulti;
#define USE_SERIAL Serial
 
//int inputPin = 2;               // choose the input pin (for PIR sensor)
//int pirState = LOW;             // we start, assuming no motion detected
//int val = 0;                    // variable for reading the pin status
 
void setup() {
  //pinMode(inputPin, INPUT);     // declare sensor as input
  //pinMode(LED_BUILTIN, OUTPUT);
  USE_SERIAL.begin(115200);
  USE_SERIAL.println();
  for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("tada", "88888888");
}
 
void loop(){
  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        //float vdd = ESP.getVcc();
        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin(, "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        http.begin("http://api.koenhabets.nl/room?enter"); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
        delay(1000);
        Serial.println("ESP8266 in sleep mode");
        ESP.deepSleep(0);
    }

    delay(10000);
}
