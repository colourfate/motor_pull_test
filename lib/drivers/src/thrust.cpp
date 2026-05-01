#include "thrust.h"
#include "HX711.h"

#define THRUST_DEFAULT_SCALE 1.0f

static HX711 *hx711 = nullptr;

bool thrustInit() {
    hx711 = new HX711();
    hx711->begin(THRUST_DT, THRUST_SCK, 128);
    if (hx711->is_ready()) {
        hx711->set_scale(THRUST_DEFAULT_SCALE);
        return true;
    }
    return false;
}

float thrustGetValue() {
    return hx711 ? hx711->get_units(5) : 0.0f;
}

void thrustTare() {
    if (hx711) hx711->tare(10);
}

void thrustCalibrate(float knownWeight) {
    if (hx711) {
        long raw = hx711->get_value(20);
        hx711->set_scale(raw / knownWeight);
    }
}

float thrustGetRaw() {
    return hx711 ? hx711->get_value(5) : 0.0f;
}

float thrustGetScale() {
    return hx711 ? hx711->get_scale() : THRUST_DEFAULT_SCALE;
}
