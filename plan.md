# Motor Pull Test Platform - Detailed Implementation Plan

## Project Overview
Build a motor pull test platform using Raspberry Pi Pico, integrating DShot throttle control, INA226 power monitoring, HX711 thrust sensing, and OLED display, with automated throttle sweep testing and data logging.

## Completed Work ✅
### Plan 1: Documentation Updates
- Updated `AGENTS.md` with hardware pinout, dependencies, key files, and full PlatformIO CLI path
- Updated `platformio.ini` with INA226 library dependency and example build environments
- Updated `requestment.md` to reflect revised requirements (INA226 instead of ADC)

### Plan 2: INA226 Power Monitor Driver
- Created `include/power_monitor.h` and `src/power_monitor.cpp` (renamed from INA226 to power_monitor)
- Created `examples/power_demo/` test example with `[env:power_demo]` build environment
- Verified build succeeds with `pio run -e power_demo`

---

## Adjusted Implementation Plan
*Adjusted per user request: Original Plan 5 (OLED Display) moved to Plan 4, Original Plan 4 (Auto Throttle Test) moved to Plan 5*

### Plan 3: Thrust Sensor Driver (HX711)
Encapsulate HX711 load cell driver as `thrust` module, no HX711 naming exposed externally.

#### 3.1 Driver Files
**`include/thrust.h`**
```cpp
#ifndef THRUST_H
#define THRUST_H

#include <Arduino.h>

#define THRUST_DT   14
#define THRUST_SCK  15

bool thrustInit();
float thrustGetValue();      // Returns thrust in grams
void thrustTare();          // Tare (no-load calibration)
void thrustCalibrate(float knownWeight);  // Calibrate with known weight
float thrustGetRaw();       // Raw reading for debugging

#endif
```

**`src/thrust.cpp`**
```cpp
#include "thrust.h"
#include "HX711.h"

static HX711 *hx711 = nullptr;

bool thrustInit() {
    hx711 = new HX711();
    hx711->begin(THRUST_DT, THRUST_SCK, 128);
    return hx711->is_ready();
}

float thrustGetValue() {
    return hx711 ? hx711->get_units(5) : 0.0f; // 5-sample average
}

void thrustTare() {
    if (hx711) hx711->tare(10); // 10-sample average tare
}

void thrustCalibrate(float knownWeight) {
    if (hx711) {
        long raw = hx711->get_value(20); // 20-sample average
        hx711->set_scale(raw / knownWeight);
    }
}

float thrustGetRaw() {
    return hx711 ? hx711->get_value(5) : 0.0f;
}
```

#### 3.2 Test & Calibration Demos
- `examples/thrust_demo/src/main.cpp`: Read thrust every second, print to serial
- `examples/calibrate_thrust/src/main.cpp`: Interactive calibration flow (tare → input known weight → calculate scale factor)
- Add `[env:thrust_demo]` and `[env:calibrate_thrust]` to `platformio.ini`

#### 3.3 Documentation Updates
- Update `AGENTS.md` key files to include `src/thrust.cpp` and `include/thrust.h`

---

### Plan 4: OLED Display Driver (Moved from Original Plan 5)
Implement SSD1306 OLED display via shared I2C bus (same as INA226).

#### 4.1 Driver Files
**`include/display.h`**
```cpp
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

bool displayInit();
void displayUpdate(float voltage, float current, float thrust);
void displayShowError(const char* msg);

#endif
```

**`src/display.cpp`**
```cpp
#include "display.h"
#include <U8g2lib.h>
#include "power_monitor.h"

// Hardware I2C, shared SDA/SCL with INA226
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, POWER_MONITOR_SDA, POWER_MONITOR_SCL);

bool displayInit() {
    if (!u8g2.begin()) return false;
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, "Motor Test");
    u8g2.sendBuffer();
    return true;
}

void displayUpdate(float voltage, float current, float thrust) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 12);
    u8g2.print("V: "); u8g2.print(voltage, 2); u8g2.print("V");
    u8g2.setCursor(0, 28);
    u8g2.print("I: "); u8g2.print(current, 3); u8g2.print("A");
    u8g2.setCursor(0, 44);
    u8g2.print("F: "); u8g2.print(thrust, 1); u8g2.print("g");
    u8g2.setCursor(0, 60);
    u8g2.print("T: "); u8g2.print(millis()/1000); u8g2.print("s");
    u8g2.sendBuffer();
}

void displayShowError(const char* msg) {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 30);
    u8g2.print("Error:");
    u8g2.setCursor(0, 45);
    u8g2.print(msg);
    u8g2.sendBuffer();
}
```

#### 4.2 Test Demo
- `examples/display_demo/src/main.cpp`: Display fixed test data
- Add `[env:display_demo]` to `platformio.ini`

#### 4.3 Documentation Updates
- Update `AGENTS.md` key files to include `src/display.cpp` and `include/display.h`
- Ensure `lib_deps` in `platformio.ini` includes `olikraus/U8g2`

---

### Plan 5: Auto Throttle Test Main Program (Moved from Original Plan 4)
Integrate all modules into `main.cpp` for automated throttle sweep testing.

#### 5.1 Functional Requirements
1. Output 0%~100% throttle via DShot (0~1000 DShot value)
2. Serial output: timestamp (ms), throttle (0-1000), voltage (V), current (A), thrust (g)
3. OLED real-time display of voltage, current, thrust

#### 5.2 Main Program Logic
**Initialization (`setup()`)**:
1. `Serial.begin(115200)`
2. `Wire.begin()` (I2C init)
3. `powerMonitorInit()` (INA226)
4. `thrustInit()` (HX711 thrust sensor)
5. `displayInit()` (OLED)
6. `dshotInit()` (DShot output)

**Test Loop (`loop()`)**:
1. Sweep throttle from 0% to 100% in configurable steps (default 50 = 5%)
2. For each throttle step:
   - Set throttle: `dshotSetThrottle(throttle)` (0~1000)
   - Stabilization delay (default 2000ms)
   - Read data:
     - Timestamp: `millis()`
     - Voltage: `powerMonitorGetVoltage()`
     - Current: `powerMonitorGetCurrent()`
     - Thrust: `thrustGetValue()`
   - Serial print: `timestamp, throttle, voltage, current, thrust`
   - Update OLED: `displayUpdate(voltage, current, thrust)`
3. Stop or loop after reaching 100% throttle (configurable)

#### 5.3 Data Output Format
```
Timestamp(ms), Throttle(0-1000), Voltage(V), Current(A), Thrust(g)
1000, 0, 12.05, 0.02, 0.0
3000, 100, 12.04, 0.15, 12.5
5000, 200, 12.03, 0.28, 25.3
...
```

#### 5.4 Integration Notes
- Replace existing `src/main.cpp` with new integrated logic
- Include all required headers: `dshot.h`, `power_monitor.h`, `thrust.h`, `display.h`

---

## File Structure
```
motor_pull_test/
├── include/
│   ├── dshot.h
│   ├── power_monitor.h
│   ├── thrust.h           # Plan 3
│   └── display.h          # Plan 4
├── src/
│   ├── main.cpp           # Plan 5 (rewrite)
│   ├── dshot.cpp
│   ├── power_monitor.cpp
│   ├── thrust.cpp         # Plan 3
│   └── display.cpp       # Plan 4
├── examples/
│   ├── dshot_demo/
│   ├── power_demo/
│   ├── thrust_demo/       # Plan 3
│   ├── calibrate_thrust/  # Plan 3
│   └── display_demo/      # Plan 4
├── lib/
│   └── pico-bidir-dshot/  # Pre-bundled DShot library
├── platformio.ini
├── AGENTS.md
├── requestment.md
└── plan.md                # This file
```

---

## Implementation Priority
1. **Plan 3**: Thrust sensor driver (no display dependency)
2. **Plan 4**: OLED display driver (independent testing, aids debugging for later steps)
3. **Plan 5**: Integrate all modules into `main.cpp`

---

## Hardware Reference
| Component | GPIO | Notes |
|-----------|------|-------|
| DShot output | 10 | DShot300 via lib/pico-bidir-dshot |
| Thrust sensor DT (HX711) | 14 | Load cell sensor |
| Thrust sensor SCK (HX711) | 15 | Load cell sensor |
| I2C SDA | 4 | Shared: INA226 + OLED SSD1306 |
| I2C SCL | 5 | Shared: INA226 + OLED SSD1306 |

## Dependencies
- `lib/pico-bidir-dshot/`: Pre-bundled bidirectional DShot library
- `RobTillaart/INA226`: I2C voltage/current sensor library
- `bogde/HX711`: Load cell sensor library
- `olikraus/U8g2`: OLED display library

## Build Commands
- Full build: `C:\Users\Shushu\.platformio\penv\Scripts\platformio.exe run`
- Build example: `C:\Users\Shushu\.platformio\penv\Scripts\platformio.exe run -e <env_name>`
- Upload: `C:\Users\Shushu\.platformio\penv\Scripts\platformio.exe run -t upload`
- Monitor: `C:\Users\Shushu\.platformio\penv\Scripts\platformio.exe device monitor`
