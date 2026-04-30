# AGENTS.md

## Dev Commands

- **Build**: `pio run` (or use VS Code PlatformIO extension)
- **Upload**: `pio run -t upload` (requires Pico in BOOTSEL mode)
- **Monitor**: `pio device monitor`

## Hardware Pinout (from requestment.md)

| Component | GPIO | Notes |
|-----------|------|-------|
| DShot output | 10 | DShot300 via lib/pico-bidir-dshot |
| HX711 DT | 14 | Load cell sensor |
| HX711 SCK | 15 | Load cell sensor |
| ADC current | 26 | Current sensing (3.3V ref) |
| ADC voltage | 27 | Voltage sensing (3.3V ref) |
| OLED SDA | 4 | SSD1306 via U8g2 |
| OLED SCL | 5 | SSD1306 via U8g2 |

## Dependencies

- `lib/pico-bidir-dshot/` - Pre-bundled bidirectional DShot library
- U8g2 - Display library (install via lib_deps if needed)
- bogde/HX711 - Load cell library (install via lib_deps if needed)

## Key Files

- `src/main.cpp` - Main application code
- `platformio.ini` - PlatformIO configuration
- `requestment.md` - Requirements specification
- `lib/pico-bidir-dshot/` - DShot driver library