#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h" 

#define PIN_LED 6     // Control signal, connect to DI of the LED
#define NUM_LED 1     // Number of LEDs in a strip

const int BouttonA = 1;
const int BouttonB = 2;
const int BouttonC = 3;

Adafruit_NeoPixel RGB_Strip = Adafruit_NeoPixel(NUM_LED, PIN_LED, NEO_GRB + NEO_KHZ800);

unsigned long lastTime = 0;
unsigned long timerDelay = 1400;
int BtA = 0, BtB = 0, BtC = 0;
int RedLight = 0, GreenLight = 0, BlueLight = 0, ledBright = 0;


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint16_t wait) {
  for (uint16_t i = 0; i < RGB_Strip.numPixels(); i++) {
    RGB_Strip.setPixelColor(i, c);
    RGB_Strip.show();
    delay(wait);
  }
}


void setup() {

  Serial.begin(9600);
  RGB_Strip.begin();
  RGB_Strip.show();
  RGB_Strip.setBrightness(200);    // Set brightness, 0-255 (darkest - brightest)

  pinMode(BouttonA, INPUT);
  pinMode(BouttonB, INPUT);
  pinMode(BouttonC, INPUT);

  wifiConnect();                  
  MQTTConnect(); 
}

void loop() {

  ClientMQTT.loop(); 
  if ((millis() - lastTime) > timerDelay) {

    if (statusR == "true")
    {
      RedLight = 255;
    }else{
      RedLight = 0;
    }
    
    if (statusG == "true")
    {
      GreenLight = 255;
    }else{
      GreenLight = 0;
    }

    if (statusB == "true")
    {
      BlueLight = 255;
    }else{
      BlueLight = 0;
    }

    //intensitee de LED
    ledBright = ledBrightFl;
    RGB_Strip.setBrightness(ledBright); 

    //allumer la led
    colorWipe(RGB_Strip.Color(RedLight, GreenLight, BlueLight), 1000);  

    //Statu des bouttons
    BtA = digitalRead(BouttonA); 
    BtB = digitalRead(BouttonB); 
    BtC = digitalRead(BouttonC);

    //send to thingsboard
    appendPayload("BT_R", BtA);
    appendPayload("BT_G", BtB);
    appendPayload("BT_B", BtC);
    sendPayload();

    lastTime = millis();
  }

}