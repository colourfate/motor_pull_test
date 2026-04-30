#include "dshot.h"

static BidirDShotX1 *esc = nullptr;
static uint32_t cached_rpm = 0;

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

void dshotUpdateTelemetry() {
    if (!esc || esc->initError()) return;

    uint32_t value = 0;
    BidirDshotTelemetryType type = esc->getTelemetryPacket(&value);
    if (type == BidirDshotTelemetryType::ERPM) {
        cached_rpm = value / (MOTOR_POLES / 2);
    }
}

uint32_t dshotReadRPM() {
    return cached_rpm;
}
