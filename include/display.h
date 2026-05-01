#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

bool displayInit();
void displayUpdate(float voltage, float current, float thrust);
void displayShowError(const char* msg);

#endif
