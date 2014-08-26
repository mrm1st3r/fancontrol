/************************************************
 * General configuration                        *
 ************************************************/

// minimum outside temperature to activate fan(s)
#define MIN_TEMP 10
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
#define SENSOR_TYPE DHT22
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
