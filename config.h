/************************************************
 * General configuration                        *
 ************************************************/

// minimum outside temperature to activate fan(s)
#define MIN_TEMP 10
// how much less humid it must be outside to operate
#define HUMID_OFFSET 1
// enable display functionality
#define USE_DISPLAY
// enable serial logging
#define USE_SERIAL
// baudrate for serial logging
#define SERIAL_BAUD 9600

/************************************************
 * Sensor configuration                         *
 ************************************************/
 
// used sensor type
#define SENSOR_TYPE_INSIDE DHT22
#define SENSOR_TYPE_OUTSIDE DHT22
// timeout between readings
#define SENSOR_TIMEOUT 2000


/************************************************
 * Port configuration                           *
 ************************************************/

// input pin for inside sensor
#define SENSOR_INSIDE_PIN 6
// input pin for outside sensor
#define SENSOR_OUTSIDE_PIN 7
// output pin for fan
#define FAN_PIN 8


/************************************************
 * Display configuration                        *
 ************************************************/

// pin configuration
#define LCD_PIN_RS     12
#define LCD_PIN_ENABLE 11
#define LCD_PIN_D4      5
#define LCD_PIN_D5      4
#define LCD_PIN_D6      3
#define LCD_PIN_D7      2
