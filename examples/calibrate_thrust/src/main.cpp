#include "thrust.h"

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    
    Serial.println("Thrust Sensor Calibration Tool");
    Serial.println("================================");
    Serial.println("This tool helps you calibrate the thrust sensor.");
    Serial.println();
    Serial.println("Steps:");
    Serial.println("1. Make sure nothing is on the load cell (tare)");
    Serial.println("2. Place a known weight on the load cell");
    Serial.println("3. Enter the known weight in grams");
    Serial.println();
    
    if (!thrustInit()) {
        Serial.println("Failed to initialize thrust sensor!");
        while (1) delay(1000);
    }
    
    Serial.println("Thrust sensor initialized.");
    Serial.println();
}

void loop() {
    Serial.println("Choose an option:");
    Serial.println("t - Tare (set zero with no load)");
    Serial.println("c - Calibrate with known weight");
    Serial.println("r - Read raw value");
    Serial.println("g - Get thrust value (grams)");
    Serial.println();
    Serial.print("Enter choice: ");
    
    while (!Serial.available()) delay(10);
    char choice = Serial.read();
    while (Serial.available()) Serial.read(); // Clear buffer
    
    Serial.println(choice);
    Serial.println();
    
    switch (choice) {
        case 't':
        case 'T':
            Serial.println("Taring... Make sure load cell is empty!");
            delay(2000);
            thrustTare();
            Serial.println("Tare complete. Zero set.");
            break;
            
        case 'c':
        case 'C':
            Serial.println("Starting calibration...");
            Serial.println("Step 1: Taring (remove any weight)...");
            delay(2000);
            thrustTare();
            Serial.println("Tare complete.");
            
            Serial.println("Step 2: Place known weight on load cell.");
            Serial.print("Enter known weight in grams: ");
            while (!Serial.available()) delay(10);
            float weight = Serial.parseFloat();
            while (Serial.available()) Serial.read();
            
            if (weight <= 0) {
                Serial.println("Invalid weight. Calibration cancelled.");
                break;
            }
            
            Serial.print("Calibrating with ");
            Serial.print(weight);
            Serial.println(" grams...");
            delay(2000);
            
            thrustCalibrate(weight);
            Serial.println("Calibration complete!");
            
            Serial.print("Current reading: ");
            Serial.print(thrustGetValue(), 2);
            Serial.println(" g");
            
            Serial.print("Current scale factor: ");
            Serial.println(thrustGetScale(), 4);
            Serial.println(">> Copy this value and update THRUST_DEFAULT_SCALE in src/thrust.cpp");
            break;
            
        case 'r':
        case 'R':
            Serial.print("Raw: ");
            Serial.print(thrustGetRaw(), 2);
            Serial.print(" (scale=");
            Serial.print(thrustGetScale(), 4);
            Serial.println(")");
            break;
            
        case 'g':
        case 'G':
            Serial.print("Thrust: ");
            Serial.print(thrustGetValue(), 2);
            Serial.print(" g (scale=");
            Serial.print(thrustGetScale(), 4);
            Serial.println(")");
            break;
            
        default:
            Serial.println("Invalid choice.");
            break;
    }
    
    Serial.println();
    delay(1000);
}
