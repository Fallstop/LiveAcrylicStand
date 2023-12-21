typedef enum WifiStateEnum {
    InitalWifi,
    ConnectingWifi,
    ConfigurationWifi,
    ConnectedWifi,
} WifiStateEnum;

void setupWifi();
WifiStateEnum loopWifi();