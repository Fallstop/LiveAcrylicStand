#include <WiFiManager.h>
#include <wifi.h>

WifiStateEnum wifiState = InitalWifi;

WiFiManager wm;

void setupWifi() {
    wifiState = ConnectingWifi;


    wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(60);

    // reset settings - wipe stored credentials for testing
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    if (wm.autoConnect()) {
        wifiState = ConnectedWifi;
    } else {
        wifiState = ConfigurationWifi;
    }
}

WifiStateEnum loopWifi() {
    if (wm.process()){
        wifiState = ConnectedWifi;
    }
    return wifiState;
}