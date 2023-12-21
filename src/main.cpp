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
	WifiConfig wifiConfig = loopWifi();

	loopPower();
	loopMQTT(wifiConfig);

	bool powerState = loopPower();
	if (powerState) {
		loopLight(wifiConfig.state);
	}
}