#include "dshot.h"

static BidirDShotX1 *esc = nullptr;

void dshotInit() {
    esc = new BidirDShotX1(DSHOT_PIN, DSHOT_SPEED);
    if (esc->initError()) {
        Serial.println("DShot init failed!");
    }
}

void dshotSetThrottle(uint16_t throttle) {
    if (esc && !esc->initError()) {
        esc->sendThrottle(throttle);
    }
}
