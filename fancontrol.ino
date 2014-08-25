/**
 * fancontrol
 * version 0.2
 * (c) 2014 Lukas 'mrmst3r' Taake
 * Published under MIT License
 *
 */

#include <DHT.h>
#include "humidity.h"

// used sensor type
#define SENSOR_TYPE DHT22

// minimum temperature to operate
#define MIN_TEMP 10

// input pin for inside sensor
#define SENSOR_INSIDE_PIN 5
// input pin for outside sensor
#define SENSOR_OUTSIDE_PIN 6
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
DHT dhtOut(SENSOR_INSIDE_PIN, SENSOR_TYPE);

void setup() {
	Serial.begin(9600);
	dhtIn.begin();
	dhtOut.begin();
	pinMode(FAN_PIN, OUTPUT);

	
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

	if (isnan(tIn) || isnan(tOut) || isnan(hIn) || isnan(hOut)) {
		Serial.println("Error while reading sensors");
		return;
	}

	// calculate absolute humidity for inside and outside
	float humidIn = absoluteHumidity(hIn, tIn);
	float humidOut = absoluteHumidity(hOut, tOut);

	int fanState = LOW;

	// simple state machine to provide easy extension and modification
	switch (state) {
		case STATE_IDLE:
			fanState = LOW;
			if (humidIn > humidOut && tOut > MIN_TEMP) {
				state = STATE_ACTIVE;
			}
			break;
		case STATE_ACTIVE:
			fanState = HIGH;
			if (humidIn <= humidOut || tOut <= MIN_TEMP) {
				state = STATE_IDLE;
			}
			break;
	}

	digitalWrite(FAN_PIN, fanState);


	Serial.print(humidIn);
	Serial.print(" | ");
	Serial.print(humidOut);
	Serial.print(" | ");
	Serial.println(state);

	delay(2000);
}

