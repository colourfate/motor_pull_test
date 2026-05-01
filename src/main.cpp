#include <Arduino.h>
#include "dshot.h"
#include "power_monitor.h"
#include "thrust.h"
#include "display.h"

// 自动油门测试参数
#define THROTTLE_STEP   50    // 油门步进 (0-2000范围，50=2.5%)
#define STABILIZE_MS    2000  // 递增阶段每个油门台阶的稳定延时(ms)
#define MAX_THROTTLE    2000  // 最大油门值
#define DECAY_STEP      50    // 递减阶段步长
#define DECAY_DELAY_MS  50    // 递减阶段每步延时(ms)

void setup() {
    Serial.begin(115200);
    delay(1000);  // 等待串口稳定
    
    Serial.println("Motor Pull Test Platform");
    Serial.println("===========================");
    
    // 初始化电源监测
    if (!powerMonitorInit()) {
        Serial.println("ERROR: Power monitor init failed!");
        displayShowError("Power Monitor");
        while (1) delay(1000);
    }
    Serial.println("Power monitor initialized.");
    
    // 初始化推力传感器
    if (!thrustInit()) {
        Serial.println("ERROR: Thrust sensor init failed!");
        displayShowError("Thrust Sensor");
        while (1) delay(1000);
    }
    Serial.println("Thrust sensor initialized.");
    
    // 初始化OLED显示
    if (!displayInit()) {
        Serial.println("ERROR: Display init failed!");
        while (1) delay(1000);
    }
    Serial.println("Display initialized.");
    
    // 初始化DShot
    dshotInit();
    Serial.println("DShot initialized.");
    
    Serial.println();
    Serial.println("Starting auto throttle test...");
    Serial.println("Timestamp(ms), Throttle(0-1000), Voltage(V), Current(A), Thrust(g)");
    Serial.println("----------------------------------------");
}

void loop() {
    static uint16_t throttle = 0;
    static bool testRunning = true;
    static bool decreasing = false;  // 是否处于递减阶段
    
    if (!testRunning) {
        delay(1000);
        return;
    }
    
    // 设置油门
    dshotSetThrottle(throttle);
    
    // 根据阶段选择延时
    if (decreasing) {
        delay(DECAY_DELAY_MS);  // 递减阶段短延时
    } else {
        delay(STABILIZE_MS);   // 递增阶段长延时
    }
    
    // 读取数据
    unsigned long timestamp = millis();
    float voltage = powerMonitorGetVoltage();
    float current = powerMonitorGetCurrent();
    float thrust = thrustGetValue();
    
    // 串口输出
    Serial.print(timestamp);
    Serial.print(", ");
    Serial.print(throttle);
    Serial.print(", ");
    Serial.print(voltage, 2);
    Serial.print(", ");
    Serial.print(current, 3);
    Serial.print(", ");
    Serial.println(thrust, 1);
    
    // OLED更新
    displayUpdate(voltage, current, thrust);
    
    // 检查阶段转换
    if (!decreasing) {
        // 递增阶段
        throttle += THROTTLE_STEP;
        if (throttle >= MAX_THROTTLE) {
            throttle = MAX_THROTTLE;  // 确保不超过最大值
            decreasing = true;      // 进入递减阶段
            Serial.println("Starting decay phase...");
        }
    } else {
        // 递减阶段
        throttle -= DECAY_STEP;
        if (throttle <= 0) {
            // 递减完成
            throttle = 0;
            dshotSetThrottle(0);
            Serial.println("Test completed!");
            testRunning = false;
            decreasing = false;
        }
    }
}
