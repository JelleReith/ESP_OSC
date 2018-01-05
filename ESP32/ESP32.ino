#include <EEPROM.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
bool state = HIGH;
void setup() {

  Serial.begin(115200);
  init_SERIAL();
  init_ESP();
  init_IR();
  awake();
  prompt_setup();
  

}

void loop() {
  if (Serial.available()) {
    serialEvent();
  }
  getMessage();
}
