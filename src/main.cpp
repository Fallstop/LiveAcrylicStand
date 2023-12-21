#include <Arduino.h>
#include <power.h>
#include <light.h>
#include <mqtt.h>

void setup() {
	Serial.begin(115200);
	setupPower();
	setupLight();
	setupWifi();
	setupMQTT();
}

void loop() {
	WifiStateEnum wifiState = loopWifi();

	loopPower();
	loopMQTT(wifiState);

	bool powerState = loopPower();
	if (powerState) {
		loopLight(wifiState);
	}
}