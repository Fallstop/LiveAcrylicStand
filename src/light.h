#pragma once

#include <wifi.h>

void lightPowerChange();
void loopLight(WifiStateEnum wifiState);
void setupLight();
void floodAtBrightness(int r, int g, int b, int brightness);
void fastBreathEffect(int r, int b, int g);