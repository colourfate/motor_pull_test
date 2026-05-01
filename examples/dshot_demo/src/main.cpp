#include <Arduino.h>
#include "dshot.h"

uint16_t g_rpm = 0;
int16_t g_step = 50;

void setup() {
    Serial.begin(115200);
    delay(5000);
    Serial.println("Motor test platform");
    dshotInit();
}

void loop() {
    dshotSetThrottle(g_rpm);
    delayMicroseconds(200);

    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 1000) {
        lastPrint = millis();
        Serial.print("RPM: ");
        Serial.println(g_rpm);
        g_rpm += g_step;
        if ((g_step > 0 && g_rpm >= 1000) ||
            (g_step < 0 && g_rpm == 0)) {
            g_step = -g_step;
        }
    }
}
