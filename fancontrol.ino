/**
 * fancontrol
 * version 0.4.1
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
#define LCD_HUMID_ABS  0
#define LCD_HUMID_REL  1
#define LCD_TEMP       2
#define LCD_TIME       3
#define LCD_TIME_TODAY 4
#define LCD_RUNTIME    5

// initial state is idle
int state = STATE_IDLE;

// inside sensor
DHT dhtIn(SENSOR_INSIDE_PIN, SENSOR_TYPE_INSIDE);
// outside sensor
DHT dhtOut(SENSOR_OUTSIDE_PIN, SENSOR_TYPE_OUTSIDE);

#ifdef USE_DISPLAY
// lcd display
LiquidCrystal lcd(
	LCD_PIN_RS,
	LCD_PIN_ENABLE,
	LCD_PIN_D4,
	LCD_PIN_D5,
	LCD_PIN_D6,
	LCD_PIN_D7);
#endif

void display();
void serial();
void countTime();

// absolute humidity values
float humidIn, humidOut;

// fan active time since last reset
unsigned long actTime = 0;
// fan active time resetted all 24 hours
unsigned long actToday = 0;
// seconds since last reset
unsigned long time = 0;
// seconds since last 24h reset
unsigned long today = 0;

void setup() {
	// initialize sensors
	dhtIn.begin();
	dhtOut.begin();
	// initialize fan output
	pinMode(FAN_PIN, OUTPUT);

#ifdef USE_DISPLAY
	// initialize d 2x16 display
	lcd.begin(16, 2);
#endif

#ifdef USE_SERIAL
	// initialize serial 
	Serial.begin(SERIAL_BAUD);
	Serial.println("Humidity based fan control");
	Serial.println("Inside | Outside | State");
#endif

#ifdef ENABLE_SWITCH
	pinMode(SWITCH_PIN, INPUT);
#endif
}

void loop() {
	// wait until sensors become ready
	delay(SENSOR_TIMEOUT);

	// read temperatures
	int tIn = dhtIn.readTemperature();
	int tOut = dhtOut.readTemperature();
	// read relative humidity
	int hIn = dhtIn.readHumidity();
	int hOut = dhtOut.readHumidity();

	char* err = NULL;

	// check for sensor failures
	if ((isnan(tIn) || isnan(hIn)) || (tIn == 0 && hIn == 0)) {
		err = "inside";
	} else if ((isnan(tOut) || isnan(hOut)) || (tOut == 0 && hOut == 0)) {
		err = "outside";
	}

	if (err != NULL) {
#ifdef USE_SERIAL
		Serial.print("Sensor fail: ");
		Serial.println(err);
#endif
#ifdef USE_DISPLAY
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("sensor fail");
		lcd.setCursor(0, 1);
		lcd.print(err);
#endif
		state = STATE_IDLE;
		return;
	}

	// calculate absolute humidity for inside and outside
	humidIn = absoluteHumidity(hIn, tIn);
	humidOut = absoluteHumidity(hOut, tOut);

	// simple state machine to provide easy extension and modification
	switch (state) {
		case STATE_IDLE:
			if ((humidIn - humidOut) >= START_DIFF && tOut > MIN_TEMP) {
				state = STATE_ACTIVE;
			}
			break;
		case STATE_ACTIVE:
			if ((humidIn - humidOut) <= STOP_DIFF || tOut <= MIN_TEMP) {
				state = STATE_IDLE;
			}
			break;
	}

#ifdef ENABLE_SWITCH
	// force active state if switch is on high
	if (digitalRead(SWITCH_PIN) == HIGH) {
		//state = STATE_ACTIVE;
		Serial.print("Knopf AN");
	}
#endif

	// Relay module works inversed
	digitalWrite(FAN_PIN, !state);

	countTime();
	display();
	serial();
}

void display() {
#ifdef USE_DISPLAY
	lcd.clear();

	// general information that is always displayed

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

	// rotate display every 6 seconds
	int lcdFlag = (millis() / 6000) % 6;

	lcd.setCursor(0, 0);
	long timeVal = -1;
	if (lcdFlag == LCD_TIME) {
		timeVal = actTime;
		lcd.print("Active time");
	} else if (lcdFlag == LCD_TIME_TODAY) {
		lcd.print("Active today");
		timeVal = actToday;
	} else if (lcdFlag == LCD_RUNTIME) {
		lcd.print("Runtime");
		timeVal = time;
	}
	// print the time that the fan was active
	if (timeVal != -1) {
		lcd.setCursor(0, 1);
		
		// hours
		int hours = timeVal / 3600;
		if (hours < 10) {
			lcd.print("0");
		}
		lcd.print(hours);
		lcd.print(":");
		// minutes
		int minutes = (timeVal / 60) - (hours * 60);
		if (minutes < 10) {
			lcd.print("0");
		}
		lcd.print(minutes);
		lcd.print(":");
		// seconds
		int seconds = timeVal % 60;
		if (seconds < 10) {
			lcd.print("0");
		}
		lcd.print(seconds);
		lcd.print("h");
		return;
	}

	float vIn = 0, vOut = 0;
	char unit = ' ';
	// basic 
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

void countTime() {
	static unsigned long lastTime = 0;
	unsigned long now = millis() / 1000;
	// pseudo rtc
	time += now - lastTime;
	today += now - lastTime;
	lastTime = now;
	if (today >= 60 * 60 * 24) {
		today = 0;
		actToday = 0;
	}

	if (state == STATE_ACTIVE) {
		actTime += 2;
		actToday += 2;
	}
}

