#include <power.h>

#include <Bounce2.h>
#include <config.h>
#include <light.h>

Bounce bounce = Bounce();

bool powerState = false;

void setupPower() {
    bounce.attach(POWER_SWITCH_PIN, INPUT_PULLUP); // USE INTERNAL PULL-UP
    // DEBOUNCE INTERVAL IN MILLISECONDS
    bounce.interval(5);

    // LED SETUP
    pinMode(POWER_LIGHT_PIN, OUTPUT);
    digitalWrite(POWER_LIGHT_PIN, powerState);
}

bool getPowerState()
{
    return powerState;
}

void loopPower()
{
    // Update the Bounce instance (YOU MUST DO THIS EVERY LOOP)
    bounce.update();

    // <Bounce>.changed() RETURNS true IF THE STATE CHANGED (FROM HIGH TO LOW OR LOW TO HIGH)
    if (bounce.changed())
    {
        int deboucedInput = bounce.read();
        powerState = deboucedInput == LOW;

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
}

void powerOnEvent() {
    Serial.println("Power on");
    lightPowerChange();
}

void powerOffEvent() {
    Serial.println("Power off");
    lightPowerChange();
}