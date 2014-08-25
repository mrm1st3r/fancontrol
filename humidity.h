/**
 * Humidity calculating library
 * Version 0.1
 *
 * (c) 2014 Lukas 'mrm1st3r' Taake
 * Published under MIT License
 *
 * Based on formulas provided by Stefan Ochs
 * http://www.wetterochs.de/
 */

/**
 * universal gas constant
 */
#define HUMID_CONST_R 8314.3

/**
 * moleculeweight of vapor
 */
#define HUMID_CONST_MW 18.016

float humidConstA(float temp);

float humidConstB(float temp);

/**
 * Calculate saturation vapor pressure.
 * @param temp temperature in °C
 * @return saturation pressure in hPa
 */
float saturationPressure(float temp);

/**
 * Calculate approximate vapor pressure.
 * @param relHumid relative humidity
 * @param temp temperature in °C
 * return vapor pressure
 */
float vaporPressure(float relHumid, float temp);

/*
 * Calculate relative humidity.
 * @param temp temperature in °C
 * @param dewPoint dew point in °C
 * @return relative humidity
 */
float relativeHumidity(float temp, float dewPoint);

/**
 * Calculate dew point temperature.
 * @param relHumid relative humidity
 * @param temp air temperature in °C
 * @return dew point temperature in °C
 */
float dewPoint(float relHumid, float temp);

/**
 * Calculate absolute humidity.
 * @param relHumid relative humidity
 * @param temp air temperature in °C
 * @return absulute humidity in g/m³
 */
float absoluteHumidity(float relHumid, float temp);

