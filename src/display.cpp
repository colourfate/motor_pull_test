#include "display.h"
#include <U8g2lib.h>
#include "power_monitor.h"

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
