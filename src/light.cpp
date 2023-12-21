#include <light.h>

#include <Adafruit_NeoPixel.h>
#include <config.h>
#include <mqtt.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_CONFIG);

int currentBrightness = 0;

void floodAtBrightness(int r, int g, int b, int brightness)
{
    if (brightness == 0) {
        strip.clear();
        strip.show();
        return;
    }
    float ratio = (float)brightness / 255.0;
    strip.fill(strip.Color(r * ratio, g * ratio, b * ratio));
    strip.show();
}

void setupLight()
{
    strip.begin();
    // Can't use for animations, highly lossy function that screws with colour
    strip.setBrightness(255);

    currentBrightness = 0;
    floodAtBrightness(GLOW_COLOUR, currentBrightness);

    strip.show();
}

void loopLight(WifiStateEnum wifiState) {
    if (wifiState == ConfigurationWifi) {
        fastBreathEffect(255,255,255);
        return;
    } else if (wifiState != ConnectedWifi) {
        fastBreathEffect(GLOW_COLOUR);
        return;
    }

    bool lightingUp = getRemoteState() || getLocalState();
    int goalBrightness = lightingUp ? STRONG_GLOW_BRIGHTNESS : WEAK_GLOW_BRIGHTNESS;
    if (currentBrightness != goalBrightness) {
        currentBrightness += (goalBrightness > currentBrightness) ? 1 : -1;
        floodAtBrightness(GLOW_COLOUR, currentBrightness);
    }
}

void lightPowerChange() {
    currentBrightness = 0;
    floodAtBrightness(GLOW_COLOUR, currentBrightness);
}

bool fastBreathEffectState = false;
void fastBreathEffect(int r, int b, int g) {
    int goalBrightness = fastBreathEffectState ? STRONG_GLOW_BRIGHTNESS : WEAK_GLOW_BRIGHTNESS;
    if (currentBrightness != goalBrightness) {
        currentBrightness += (goalBrightness > currentBrightness) ? 2 : -2;
        floodAtBrightness(r, b, g, currentBrightness);
    } else {
        fastBreathEffectState = !fastBreathEffectState;
    }
}