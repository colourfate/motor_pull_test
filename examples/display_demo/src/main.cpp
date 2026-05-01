#include "display.h"
#include "power_monitor.h"

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    
    Serial.println("Display Demo");
    Serial.println("Initializing display...");
    
    if (!displayInit()) {
        Serial.println("Failed to initialize display!");
        while (1) delay(1000);
    }
    
    Serial.println("Display initialized.");
    Serial.println("Displaying test data...");
}

void loop() {
    // Display test data
    displayUpdate(12.05, 1.234, 123.5);
    delay(2000);
    
    // Change values for demo
    static float counter = 0;
    counter += 0.1;
    if (counter > 100) counter = 0;
    
    displayUpdate(12.0 + (counter/100), 0.5 + (counter/200), counter * 10);
    delay(2000);
}
