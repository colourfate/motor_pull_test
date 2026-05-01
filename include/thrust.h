#ifndef THRUST_H
#define THRUST_H

#include <Arduino.h>

#define THRUST_DT   14
#define THRUST_SCK  15

bool thrustInit();
float thrustGetValue();
void thrustTare();
void thrustCalibrate(float knownWeight);
float thrustGetRaw();
float thrustGetScale();

#endif
