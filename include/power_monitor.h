#ifndef POWER_MONITOR_H
#define POWER_MONITOR_H

#include <Arduino.h>
#include <Wire.h>
#include "INA226.h"

#define POWER_MONITOR_SDA     4
#define POWER_MONITOR_SCL     5
#define POWER_MONITOR_ADDR    0x40

bool powerMonitorInit();
float powerMonitorGetVoltage();
float powerMonitorGetCurrent();
float powerMonitorGetPower();

#endif
