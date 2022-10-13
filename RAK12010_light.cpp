/**
 * @file RAK12010_light.cpp
 * @author Bernd Giesecke (bernd.giesecke@rakwireless.com)
 * @brief Functions for RAK12010 light sensor
 * @version 0.1
 * @date 2022-02-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "RAK12010_light.h"

/** Light sensor instance */
Light_VEML7700 VEML = Light_VEML7700();

/**
 * @brief Initialize light sensor
 *
 * @return true success
 * @return false failed
 */
bool init_rak12010(void)
{
	pinMode(WB_IO2, OUTPUT);
	digitalWrite(WB_IO2, HIGH);

	Wire.begin();
	if (!VEML.begin(&Wire))
	{
		Serial.println("VEML7700 not found");
		return false;
	}

	Serial.println("Found VEML7700");
	VEML.setGain(VEML7700_GAIN_1_8);
	VEML.setIntegrationTime(VEML7700_IT_25MS);

	// VEML.powerSaveEnable(true);
	// VEML.setPowerSaveMode(VEML7700_POWERSAVE_MODE4);
	Serial.printf("First reading %.2f Lux\n", VEML.readLux());
	return true;
}

/**
 * @brief Read value from light sensor
 *     Data is added to Cayenne LPP payload as channel
 *     LPP_CHANNEL_LIGHT2
 *
 */
float read_rak12010(void)
{
	float light_lux = VEML.readLux();

	return light_lux;
}