#include <power.h>

#include <Bounce2.h>
#include <config.h>
#include <light.h>

Bounce powerButton = Bounce();

bool powerState = false;

void setupPower() {
    powerButton.attach(POWER_SWITCH_PIN, INPUT_PULLUP); // USE INTERNAL PULL-UP
    // DEBOUNCE INTERVAL IN MILLISECONDS
    powerButton.interval(5);

    powerState = powerButton.read() == HIGH;

    // LED SETUP
    pinMode(POWER_LIGHT_PIN, OUTPUT);
    digitalWrite(POWER_LIGHT_PIN, powerState);
}

bool loopPower()
{
    // Update the Bounce instance (YOU MUST DO THIS EVERY LOOP)
    powerButton.update();

    // <Bounce>.changed() RETURNS true IF THE STATE CHANGED (FROM HIGH TO LOW OR LOW TO HIGH)
    if (powerButton.changed())
    {
        int deboucedInput = powerButton.read();
        powerState = deboucedInput == HIGH;

        digitalWrite(POWER_LIGHT_PIN, powerState); // WRITE THE NEW ledState
        if (powerState)
        {
            powerOnEvent();
        }
        else
        {
            powerOffEvent();
        }
    }

    return powerState;
}

void powerOnEvent() {
    Serial.println("Power on");
    lightPowerChange();
}

void powerOffEvent() {
    Serial.println("Power off");
    lightPowerChange();
}