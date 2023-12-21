#pragma once

#include <Adafruit_NeoPixel.h>

constexpr const char MQTT_BROKER_DEFAULT[] = "broker.hivemq.com";
constexpr const char MQTT_PORT_DEFAULT[] = "1883";
constexpr const char MQTT_TOPIC[] = "liveacrylicstand/";
constexpr const char MQTT_CHANNEL_DEFAULT[] = "12345";

constexpr int POWER_SWITCH_PIN = 5; // D1
constexpr int POWER_LIGHT_PIN = 16; // D0

constexpr int PRIMARY_BUTTON_PIN = 14; // D5

constexpr int NEOPIXEL_PIN = 4; // D2
constexpr int NEOPIXEL_COUNT = 24;
constexpr int NEOPIXEL_CONFIG = NEO_GRB + NEO_KHZ800;

#define GLOW_COLOUR 255, 89, 0
constexpr int STRONG_GLOW_BRIGHTNESS = 255;
constexpr int WEAK_GLOW_BRIGHTNESS = 100;

constexpr int WIFI_CONFIG_LENGTH = 40;