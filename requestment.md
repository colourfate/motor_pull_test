# Requirements document of motor pull speed measurement

## OR
Platform for automatically teste motor pull
## Hardware
1. MCU
Use Raspberry Pi rp2040 as the main controller.
2. sensor
Test the motor pull with HX711, and configure bogde/HX711 library to complete the function. Use gpio14
as the DT interface of the sensor, and gpio15 as the SCK interface.
3. output
Use Dshot300 as an output protocol to control external electric modulation. lib/pico-bidir-dshot open source library
has been downloaded to complete dshot control, and use GPIO 10 as output pin.
4. input
Use ADC to sample the current power supply voltage and current, the reference voltage is 3.3V, use GPIO 26
to sample the current, and use GPIO 27 to sample the voltage.
5. display
An oled screen is used to display the current voltage, current and throttle, the screen size is 0.96 inch, and the sensor
is sd1306, which is completed by olikraus/U8g2. SDA of i2c is assigned to GPIO 4, and scl is assigned to GPIO 5.

## Featrue
1. Automatically output 0%~100% throttle through dshot, and print the current throttle size, time stamp, voltage,
current and motor pull through uart
2. The OLED screen displays the current current, voltage and motor pull values.
