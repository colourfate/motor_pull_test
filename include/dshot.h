#ifndef DSHOT_H
#define DSHOT_H

#include <Arduino.h>
#include <PIO_DShot.h>

#define DSHOT_PIN       10
#define DSHOT_SPEED     300
#define MOTOR_POLES     12

void dshotInit();
void dshotSetThrottle(uint16_t throttle);

#endif
