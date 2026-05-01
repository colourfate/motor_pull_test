#include <Arduino.h>
#include "power_monitor.h"

void setup() {
    Serial.begin(115200);
    delay(5000);
    Serial.println("Power Monitor Demo");
    
    if (!powerMonitorInit()) {
        Serial.println("Power monitor init failed!");
        while(1) delay(1000);
    }
    Serial.println("Power monitor init successful!");
}

void loop() {
    float voltage = powerMonitorGetVoltage();
    float current = powerMonitorGetCurrent();
    float power = powerMonitorGetPower();
    
    Serial.print("Voltage: ");
    Serial.print(voltage, 3);
    Serial.print(" V, Current: ");
    Serial.print(current, 3);
    Serial.print(" A, Power: ");
    Serial.print(power, 3);
    Serial.println(" W");
    
    delay(1000);
}
