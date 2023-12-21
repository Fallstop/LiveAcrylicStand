void newMessageCallback(char *topic, byte *payload, unsigned int length);
void reconnect();
void setupMQTT();
bool getRemoteState();
bool getLocalState();
void sendStateMQTT(bool newState);
void loopMQTT(WifiStateEnum wifiState);