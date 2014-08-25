/**
 * Humidity calculating library
 * Version 0.1
 *
 * (c) 2014 Lukas 'mrm1st3r' Taake
 * Published under MIT License
 *
 * Based on the formulas provided by Stefan Ochs
 * http://www.wetterochs.de/
 */
 
#include "humidity.h"
#include <math.h>

float humidConstA(float temp) {
	if (temp < 0) {
		return 7.6;
	} else {
		return 7.5;
	}
}

float humidConstB(float temp) {
	if (temp < 0) {
		return 237.3;
	} else {
		return 240.7;
	}
}

float saturationPressure(float temp) {
	return 6.1078 * pow(10, (humidConstA(temp)*temp) / (humidConstB(temp) + temp));
}

float vaporPressure(float relHumid, float temp){
	return relHumid / 100 * saturationPressure(temp);
}

float relativeHumidity(float temp, float dewPoint) {
	return 100 * saturationPressure(dewPoint) / saturationPressure(temp); 
}

float v(float temp, float relHumid) {
	return log10(vaporPressure(relHumid, temp) / 6.1078);
}

float dewPoint(float relHumid, float temp) {
	return humidConstB(temp) * v(temp, relHumid)
			/ (humidConstA(temp) - v(temp, relHumid));
}

float absoluteHumidity(float relHumid, float temp) {
	return pow(10, 5) * HUMID_CONST_MW / HUMID_CONST_R
			* vaporPressure(relHumid, temp) / (temp + 273.15);
}

