/**
 * fancontrol
 * version 0.2
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

// minimum temperature to operate
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

// initial state is idle
int state = STATE_IDLE;

// inside sensor
DHT dhtIn(SENSOR_INSIDE_PIN, SENSOR_TYPE);
// outside
DHT dhtOut(SENSOR_OUTSIDE_PIN, SENSOR_TYPE);

// 2x16 display
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

	// inside humidity
	lcd.setCursor(0, 0);
	lcd.print("in: ");
	// correct number alignment
	if (humidIn < 10) {
		lcd.print(" ");
	}
	lcd.print(humidIn);
	
	// outside humidity
	lcd.setCursor(0, 1);
	lcd.print("out:");
	// correct number alignment
	if (humidOut < 10) {
		lcd.print(" ");
	}
	lcd.print(humidOut);

	// active state
	if (state == STATE_ACTIVE) {
		lcd.setCursor(10, 0);
		lcd.print("active");
	}
	// outside lower than min temperature
	if (tOut < MIN_TEMP) {
		lcd.setCursor(12, 1);
		lcd.print("cold");
	}

	// serial data output
	Serial.print(humidIn);
	Serial.print(" | ");
	Serial.print(humidOut);
	Serial.print(" | ");
	Serial.println(state);

	// sensors can only be read every 2 seconds
	delay(SENSOR_TIMEOUT);
}

