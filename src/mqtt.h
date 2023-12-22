#pragma once
#include <wifi.h>

void newMessageCallback(char *topic, unsigned char *payload, unsigned int length);
void reconnect();
void setupMQTT();
bool getRemoteState();
bool getLocalState();
void sendStateMQTT(bool newState);
void loopMQTT(WifiConfig wifiConfig);

void setMQTTKey(WifiConfig wifiConfig);