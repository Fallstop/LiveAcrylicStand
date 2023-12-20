#include <Arduino.h>
#include <power.h>

void setup() {
  setupPower();
}

void loop() {
  // put your main code here, to run repeatedly:
  loopPower();
  if (getPowerState()) {
    
  }
}