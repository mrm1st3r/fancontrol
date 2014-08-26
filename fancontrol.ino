/**
 * fancontrol
 * version 0.3.1
 * (c) 2014 Lukas 'mrmst3r' Taake
 * Published under MIT License
 *
 */

#include <DHT.h>
#include <LiquidCrystal.h>
#include "humidity.h"

// used sensor type
#define SENSOR_TYPE DHT22
// timeout between readings
#define SENSOR_TIMEOUT 2000

// minimum outside temperature to activate fan(s)
#define MIN_TEMP 10

// input pin for inside sensor
#define SENSOR_INSIDE_PIN 6
// input pin for outside sensor
#define SENSOR_OUTSIDE_PIN 7
// output pin for fan
#define FAN_PIN 8

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
// outside
DHT dhtOut(SENSOR_OUTSIDE_PIN, SENSOR_TYPE);

// lcd display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
	Serial.begin(9600);
	dhtIn.begin();
	dhtOut.begin();
	lcd.begin(16, 2);
	pinMode(FAN_PIN, OUTPUT);

	// Header for serial data	
	Serial.println("Humidity based fan control");
	Serial.println("Inside | Outside | State");
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
		Serial.println("Error while reading sensors");
		return;
	}

	// calculate absolute humidity for inside and outside
	float humidIn = absoluteHumidity(hIn, tIn);
	float humidOut = absoluteHumidity(hOut, tOut);

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

	/********************************
	 * Display output               *
	 ********************************/
	lcd.clear();
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
			vIn = hIn;
			vOut = hOut;
			unit = '%';
			break;
		case LCD_TEMP:
			vIn = tIn;
			vOut = tOut;
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
	if (tOut < MIN_TEMP) {
		lcd.setCursor(12, 1);
		lcd.print("cold");
	}

	/***************************
	 * Serial Output           *
	 ***************************/

	Serial.print(humidIn);
	Serial.print(" | ");
	Serial.print(humidOut);
	Serial.print(" | ");
	Serial.println(state);

	// wait until sensors become ready
	delay(SENSOR_TIMEOUT);
}

