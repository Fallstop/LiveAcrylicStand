#pragma once

typedef enum WifiStateEnum {
    InitalWifi,
    ConnectingWifi,
    ConfigurationWifi,
    ConnectedWifi,
} WifiStateEnum;

typedef struct WifiConfig {
    const WifiStateEnum state;
    const char * mqtt_server;
    const int mqtt_port;
    const char * mqtt_channel_broadcast;
    const char * mqtt_channel_listen;
} WifiConfig;

void setupWifi();
WifiConfig loopWifi();

