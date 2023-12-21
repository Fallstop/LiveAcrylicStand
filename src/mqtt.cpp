#include <config.h>
#include <wifi.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Bounce2.h>

bool remoteClientState = false;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

Bounce primaryButton = Bounce();

bool mqttConfigured = false;

void newMessageCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.print((char)payload[0]);
    Serial.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
        remoteClientState = true;
    }
    else if ((char)payload[0] == '0')
    {
        remoteClientState = false;
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("outTopic", "hello world");
            // ... and resubscribe
            client.subscribe("inTopic");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setupMQTT()
{
    // MQTT will only configure when in the main loop, once configured by Wifi Portal
    randomSeed(micros());

    primaryButton.attach(PRIMARY_BUTTON_PIN, INPUT_PULLUP); // USE INTERNAL PULL-UP
    // DEBOUNCE INTERVAL IN MILLISECONDS
    primaryButton.interval(5);
}

bool getRemoteState()
{
    return remoteClientState;
}

bool getLocalState()
{
    return primaryButton.read() == LOW;
}

void sendStateMQTT(bool newState, WifiConfig wifiConfig)
{
    char mqtt_key[WIFI_CONFIG_LENGTH + sizeof(MQTT_TOPIC)];
    sprintf(mqtt_key, "%s%s" , MQTT_TOPIC, wifiConfig.mqtt_channel);
    client.publish(mqtt_key, newState ? "1" : "0");
}

void loopMQTT(WifiConfig wifiConfig)
{

    if (wifiConfig.state != ConnectedWifi)
    {
        return;
    }
    
    if (!mqttConfigured) {
        client.setServer(wifiConfig.mqtt_server, wifiConfig.mqtt_port);
        client.setCallback(newMessageCallback);
        mqttConfigured = true;
    }

    if (!client.connected())
    {
        reconnect();
    }

    client.loop();

    primaryButton.update();

    if (primaryButton.changed())
    {
        sendStateMQTT(primaryButton.read() == LOW, wifiConfig);
    }

    unsigned long now = millis();
    if (now - lastMsg > 2500)
    {
        // Every 2.5s, resend current state
        sendStateMQTT(primaryButton.read() == LOW, wifiConfig);
        lastMsg = now;
    }
}