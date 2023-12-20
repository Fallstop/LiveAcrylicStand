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

void loopLight() {
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