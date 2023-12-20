#include <light.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEOPIXEL_CONFIG);

int currentBrightness = 0;

void floodAtBrightness(int r, int g, int b, int brightness)
{
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
    if (lightingUp && currentBrightness < STRONG_GLOW_BRIGHTNESS) {
        currentBrightness += 1;
    } else if (!lightingUp && currentBrightness > WEAK_GLOW_BRIGHTNESS) {
        currentBrightness -= 1;
    }
}