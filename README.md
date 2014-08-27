# fancontrol
## General
This program controls a fan to lower air humidity e.g. in basements.
It uses two combined temperature and humidity sensors to calculate
the approximate absolute humidity inside and outside.
If the absolute humidity outside is lower than inside and the outside
temperature is above the minimum temperature, the fan pin gets powered.

## Features
* configurable offset in humidity to operate
* configurable minimum outside temperature to activate fan
* many values can be shown on display
    * absolute humidity (inside + outside)
    * relative humidity (inside + outside)
    * temperature (inside + outside)
    * time that the fan was active (today + since last reset)
    * runtime
* logging via serial over USB
* different DHT sensors may be used for in and outside (if they are compatible with the library)
* flexible port configuration

## Dependencies
* DHT sensors: To read the sensors, Adafruits DHT sensor library is used and
has to be downloaded seperately.
https://github.com/adafruit/DHT-sensor-library
* Display: The standard LiquidCrystal library is used to comminucate with
the display

## Hardware
To operate properly the following hardware are being used:
* 1x Arduino Uno
* 2x DHT22 temperature and humidity sensors
* 1x relay board to power up fan(s)
* 1x any 16x2 LCD display that is supported by the LiquidCrystal library (tested with JHD162a)
    * peripherals (e.g. potentiometer for contrast current)

## Connection
The listed hardware should be connected to the Arduino as follows
(numbers are arduino ports):

2  => LCD DB7  
3  => LCD DB6  
4  => LCD DB5  
5  => LCD DB4  
6  => DHT data (inside)  
7  => DHT data (outside)  
8  => Relay  
11 => LCD Enable  
12 => LCD RS

(alternatively ports can be changed in software)

Additional connections to LCD ports (for JHD162a):

1  => GND  
2  => VCC 5V  
3  => Contrast Current  
5  => GND  
15 => VCC 4.2V (Backlight)  
16 => GND (Backlight)

## Configuration
You may decide what functionality exact you want by modifying config.h.  

