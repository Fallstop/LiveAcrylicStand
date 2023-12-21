#include <WiFiManager.h>
#include <wifi.h>
#include <config.h>
#include <ArduinoJson.h>

WifiStateEnum wifiState = InitalWifi;

WiFiManager wm;


WiFiManagerParameter custom_mqtt_server("mqtt_server", "MQTT Host", MQTT_BROKER_DEFAULT, WIFI_CONFIG_LENGTH);
WiFiManagerParameter custom_mqtt_port("mqtt_port", "MQTT Port", MQTT_PORT_DEFAULT, WIFI_CONFIG_LENGTH);
WiFiManagerParameter custom_mqtt_channel("mqtt_channel", "MQTT Channel", MQTT_CHANNEL_DEFAULT, WIFI_CONFIG_LENGTH);

void setupWifi() {
    custom_mqtt_server.setValue(MQTT_BROKER_DEFAULT, WIFI_CONFIG_LENGTH);
    custom_mqtt_port.setValue(MQTT_PORT_DEFAULT, WIFI_CONFIG_LENGTH);
    custom_mqtt_channel.setValue(MQTT_CHANNEL_DEFAULT, WIFI_CONFIG_LENGTH);

    wifiState = ConnectingWifi;

    wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(60);
    wm.setDarkMode(true);
    wm.setCountry("NZ");

    
    
    

    wm.addParameter(&custom_mqtt_server);
    wm.addParameter(&custom_mqtt_port);
    wm.addParameter(&custom_mqtt_channel);

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

WifiConfig loopWifi() {
    if (wm.process()){
        wifiState = ConnectedWifi;

    }
    return WifiConfig {
        .state = wifiState,
        .mqtt_server = custom_mqtt_server.getValue(),
        .mqtt_port = atoi(custom_mqtt_port.getValue()),
        .mqtt_channel = custom_mqtt_channel.getValue(),
    };
}

int loadConfig() {
    Serial.println("Loading config");
    if (SPIFFS.begin()) {
        Serial.println(" -> Mounted file system");
        if (SPIFFS.exists("/config.json")) {
            //file exists, reading and loading
            Serial.println(" -> Reading config file");
            File configFile = SPIFFS.open("/config.json", "r");
            if (configFile) {
                Serial.println(" -> Opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);
                configFile.readBytes(buf.get(), size);
                DynamicJsonDocument doc(1024);
                auto error = deserializeJson(doc, buf.get());
                if (error) {
                    Serial.println(" -> Failed to load json config");
                } else {
                    Serial.println(" -> Successfully parsed config");

                    custom_mqtt_server.setValue(doc["mqtt_server"], strlen(doc["mqtt_server"]));
                    custom_mqtt_port.setValue(doc["mqtt_port"], strlen(doc["mqtt_port"]));
                    custom_mqtt_channel.setValue(doc["mqtt_channel"], strlen(doc["mqtt_channel"]));

                    Serial.print(" -> MQTT URI: ");
                    Serial.printf("%s:%s\n", custom_mqtt_server.getValue(), custom_mqtt_port.getValue());
                    Serial.print(" -> MQTT channel: ");
                    Serial.printf("%s\n", custom_mqtt_channel.getValue());
                    
                }
                configFile.close();
            }
        }
    } else {
        Serial.println("failed to mount FS");
        return 1;
    }
    return 0;
}

void saveConfig() {
    Serial.println("Saving config");
    DynamicJsonDocument doc(1024);
    doc["mqtt_server"] = custom_mqtt_server.getValue();
    doc["mqtt_port"] = custom_mqtt_port.getValue();
    doc["mqtt_channel"] = custom_mqtt_channel.getValue();

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
    }
    serializeJson(doc, configFile);
    configFile.close();
    Serial.println("Saved config");
}