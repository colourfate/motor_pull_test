# AGENTS.md

## Dev Commands

- **Build**: `pio run` (or use VS Code PlatformIO extension)
- **Upload**: `pio run -t upload` (requires Pico in BOOTSEL mode)
- **Monitor**: `pio device monitor`
- **Build example**: `pio run -e <env_name>`

## Hardware Pinout (from requestment.md)

| Component | GPIO | Notes |
|-----------|------|-------|
| DShot output | 10 | DShot300 via lib/pico-bidir-dshot |
| HX711 DT | 14 | Load cell sensor |
| HX711 SCK | 15 | Load cell sensor |
| I2C SDA | 4 | Shared: INA226 + OLED SSD1306 |
| I2C SCL | 5 | Shared: INA226 + OLED SSD1306 |

## Dependencies

- `lib/pico-bidir-dshot/` - Pre-bundled bidirectional DShot library
- U8g2 - Display library (install via lib_deps if needed)
- bogde/HX711 - Load cell library (install via lib_deps if needed)
- RobTillaart/INA226 - I2C voltage/current sensor library (install via lib_deps)

## Key Files

- `src/main.cpp` - Main application code (auto throttle test)
- `src/dshot.cpp` + `include/dshot.h` - DShot output driver
- `src/power_monitor.cpp` + `include/power_monitor.h` - INA226 voltage/current sensor driver
- `src/thrust.cpp` + `include/thrust.h` - Thrust sensor driver (HX711 load cell)
- `src/display.cpp` + `include/display.h` - OLED display driver
- `platformio.ini` - PlatformIO configuration
- `requestment.md` - Requirements specification
- `lib/pico-bidir-dshot/` - DShot driver library