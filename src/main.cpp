#include <Arduino.h>
#include "dshot.h"

void setup() {
    Serial.begin(115200);
    delay(3000);
    Serial.println("Motor test platform");
    dshotInit();
}

void loop() {
    dshotUpdateTelemetry();
    dshotSetThrottle(0);
    delayMicroseconds(200);

    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 1000) {
        lastPrint = millis();
        Serial.print("RPM: ");
        Serial.println(dshotReadRPM());
    }
}
