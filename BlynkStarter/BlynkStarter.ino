/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP32 chip.

  Note: This requires ESP32 support package:
    https://github.com/espressif/arduino-esp32

  Please be sure to select the right ESP32 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

char auth[] = "osAEBhzUq3HIteqfyXEemA3m-5aRujtL";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 

char ssid[32] = "iPhone 9";
char pass[32] = "alvin123";

#define LED 2

BlynkTimer timer;
int time_count = 0;
String content = "";

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;

int LED_State = 0;

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(LED, OUTPUT);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LED, ledChannel);
  timer.setInterval(1000L, myTimerEvent);
}

BLYNK_WRITE(2)
{
  if(LED_State){
    int val = param.asInt();
    ledcWrite(ledChannel, val);
  }
}

BLYNK_WRITE(V1)
{
    int pinValue = param.asInt();

    if (pinValue == 0) {
        LED_State = 0;
        ledcWrite(ledChannel, pinValue);  
    }
    else {
        LED_State = 1;  
    }
}

void myTimerEvent()
{
    Blynk.virtualWrite(V0, millis() / 1000);
    if (time_count == 100){
        time_count = 0;
    }
    else {
        char character;
        while(Serial.available()) {
            character = Serial.read();
            content.concat(character);
        }
        if (content != "") {
            Blynk.virtualWrite(V3, content);
            content = "";
        }  
    }
    time_count += 1;
}

void loop()
{
  Blynk.run();
  timer.run();
}
