#include "thrust.h"

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    
    Serial.println("Thrust Sensor Demo");
    Serial.println("Initializing thrust sensor...");
    
    if (!thrustInit()) {
        Serial.println("Failed to initialize thrust sensor!");
        while (1) delay(1000);
    }
    
    Serial.println("Thrust sensor initialized.");
    Serial.println("Reading thrust values every second...");
    Serial.println("Timestamp(ms), Thrust(g)");
}

void loop() {
    unsigned long timestamp = millis();
    float thrust = thrustGetValue();
    
    Serial.print(timestamp);
    Serial.print(", ");
    Serial.println(thrust, 2);
    
    delay(1000);
}
