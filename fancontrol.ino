/**
 * fancontrol
 * version 0.3.1
 * (c) 2014 Lukas 'mrmst3r' Taake
 * Published under MIT License
 */

#include <DHT.h>
#include <LiquidCrystal.h>
#include "humidity.h"
#include "config.h"

// all possible states
#define STATE_IDLE 0
#define STATE_ACTIVE 1

// all possible display modes
#define LCD_HUMID_ABS 0
#define LCD_HUMID_REL 1
#define LCD_TEMP      2

// initial state is idle
int state = STATE_IDLE;

// inside sensor
DHT dhtIn(SENSOR_INSIDE_PIN, SENSOR_TYPE);
// outside sensor
DHT dhtOut(SENSOR_OUTSIDE_PIN, SENSOR_TYPE);

#ifdef USE_DISPLAY
// lcd display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#endif

void display();
void serial();

float humidIn, humidOut;

void setup() {
	// initialize sensors
	dhtIn.begin();
	dhtOut.begin();
	// initialize fan output
	pinMode(FAN_PIN, OUTPUT);

#ifdef USE_DISPLAY
	// initialize display
	lcd.begin(16, 2);
#endif

#ifdef USE_SERIAL
	// initialize serial 
	Serial.begin(9600);
	Serial.println("Humidity based fan control");
	Serial.println("Inside | Outside | State");
#endif
}

void loop() {
	// read temperatures
	int tIn = dhtIn.readTemperature();
	int tOut = dhtOut.readTemperature();
	// read relative humidity
	int hIn = dhtIn.readHumidity();
	int hOut = dhtOut.readHumidity();

	// check for reading errors
	if (isnan(tIn) || isnan(tOut) || isnan(hIn) || isnan(hOut)) {
#ifdef USE_SERIAL
		Serial.println("Error while reading sensors");
#endif
		return;
	}

	// calculate absolute humidity for inside and outside
	humidIn = absoluteHumidity(hIn, tIn);
	humidOut = absoluteHumidity(hOut, tOut);

	// simple state machine to provide easy extension and modification
	switch (state) {
		case STATE_IDLE:
			if (humidIn > humidOut && tOut > MIN_TEMP) {
				state = STATE_ACTIVE;
			}
			break;
		case STATE_ACTIVE:
			if (humidIn <= humidOut || tOut <= MIN_TEMP) {
				state = STATE_IDLE;
			}
			break;
	}

	// Relay module works inversed
	digitalWrite(FAN_PIN, !state);
	
	display();
	serial();

	// wait until sensors become ready
	delay(SENSOR_TIMEOUT);
}

void display() {
#ifdef USE_DISPLAY
	lcd.clear();
	// rotate display every 6 seconds
	int lcdFlag = (millis() / 6000) % 3;

	float vIn = 0, vOut = 0;
	char unit = ' ';
	switch (lcdFlag) {
		case LCD_HUMID_ABS:
			vIn = humidIn;
			vOut = humidOut;
			// full unit would be g/m³ but is to long for the display
			unit = 'g';
			break;
		case LCD_HUMID_REL:
			vIn = dhtIn.readHumidity();
			vOut = dhtOut.readHumidity();
			unit = '%';
			break;
		case LCD_TEMP:
			vIn = dhtIn.readTemperature();
			vOut = dhtOut.readTemperature();
			unit = 'C';
			break;
	}

	// inside value
	lcd.setCursor(0, 0);
	lcd.print("in:  ");
	// correct number alignment
	if (vIn < 10) {
		lcd.print(" ");
	}
	lcd.print(vIn);
	lcd.write(unit);
	
	// outside value
	lcd.setCursor(0, 1);
	lcd.print("out: ");
	// correct number alignment
	if (vOut < 10) {
		lcd.print(" ");
	}
	lcd.print(vOut);
	lcd.write(unit);

	// current state
	if (state == STATE_ACTIVE) {
		lcd.setCursor(13, 0);
		lcd.print("act");
	}
	// outside temperature lower than minimum
	if (dhtOut.readTemperature() < MIN_TEMP) {
		lcd.setCursor(12, 1);
		lcd.print("cold");
	}
#endif // USE_DISPLAY
}

void serial() {
#ifdef USE_SERIAL
	Serial.print(humidIn);
	Serial.print(" | ");
	Serial.print(humidOut);
	Serial.print(" | ");
	Serial.println(state);
#endif
}

