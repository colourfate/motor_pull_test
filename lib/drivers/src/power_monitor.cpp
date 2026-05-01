#include "power_monitor.h"

static INA226 *ina226 = nullptr;

bool powerMonitorInit() {
    Wire.begin();
    
    ina226 = new INA226(POWER_MONITOR_ADDR, &Wire);
    if (!ina226->begin()) {
        Serial.println("Power monitor init failed!");
        return false;
    }
    
    ina226->setAverage(INA226_16_SAMPLES);
    ina226->setBusVoltageConversionTime(INA226_1100_us);
    ina226->setShuntVoltageConversionTime(INA226_1100_us);
    ina226->setModeShuntBusContinuous();
    ina226->setMaxCurrentShunt(2.0, 0.01);
    
    return true;
}

float powerMonitorGetVoltage() {
    if (ina226) {
        return ina226->getBusVoltage();
    }
    return 0.0;
}

float powerMonitorGetCurrent() {
    if (ina226) {
        return ina226->getCurrent();
    }
    return 0.0;
}

float powerMonitorGetPower() {
    if (ina226) {
        return ina226->getPower();
    }
    return 0.0;
}
