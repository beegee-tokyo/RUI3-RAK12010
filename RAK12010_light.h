/**
 * @file RAK12010_light.h
 * @author Bernd Giesecke (bernd@giesecke.tk)
 * @brief Global definitions and forward declarations
 * @version 0.1
 * @date 2022-09-23
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef RAK12010_H
#define RAK12010_H
#include <Arduino.h>
#include <Light_VEML7700.h> // To install click here: http://librarymanager/All#Light_veml7700

bool init_rak12010(void);
float read_rak12010();

#endif // RAK12010_H