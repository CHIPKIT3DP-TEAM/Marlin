/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * @zawata
 * Shield3DP Pinout
 */

#ifndef BOARD_NAME
  #define BOARD_NAME "Shield3DP B0"
#endif

//
// Servos
//
// SERVO0_PIN
// SERVO1_PIN
// SERVO2_PIN
// SERVO3_PIN
#define SERV0_PIN 32

//
// Limit Switches
//
// X_MIN_PIN
// X_MAX_PIN
// Y_MIN_PIN
// Y_MAX_PIN
// Z_MIN_PIN
// Z_MAX_PIN
#define X_MIN_PIN 20
#define X_MAX_PIN 36
#define Y_MIN_PIN 21
#define Y_MAX_PIN 35
#define Z_MIN_PIN 22
#define Z_MAX_PIN 34


//
// Z Probe (when not Z_MIN_PIN)
//
// Z_MIN_PROBE_PIN

//
// Steppers
//
// X_STEP_PIN
// X_DIR_PIN
// X_ENABLE_PIN
// X_CS_PIN
#define X_STEP_PIN   3
#define X_DIR_PIN    2
#define X_ENABLE_PIN 33

// Y_STEP_PIN
// Y_DIR_PIN
// Y_ENABLE_PIN
// Y_CS_PIN
#define Y_STEP_PIN   5
#define Y_DIR_PIN    4
#define Y_ENABLE_PIN 33

// Z_STEP_PIN
// Z_DIR_PIN
// Z_ENABLE_PIN
// Z_CS_PIN
#define Z_STEP_PIN   7
#define Z_DIR_PIN    6
#define Z_ENABLE_PIN 33

// E0_STEP_PIN
// E0_DIR_PIN
// E0_ENABLE_PIN
// E0_CS_PIN
#define E0_STEP_PIN   8
#define E0_DIR_PIN    9
#define E0_ENABLE_PIN 33

//
// Temperature Sensors
//
// TEMP_0_PIN
// TEMP_1_PIN
// TEMP_2_PIN
// TEMP_3_PIN
// TEMP_4_PIN
// TEMP_BED_PIN
// TEMP_CHAMBER_PIN
#define TEMP_0_PIN   16   // Analog Input
#define TEMP_BED_PIN 15   // Analog Input

//
// Heaters / Fans
//
// HEATER_0_PIN
// HEATER_1_PIN
// HEATER_2_PIN
// HEATER_3_PIN
// HEATER_4_PIN
// HEATER_BED_PIN
// FAN_PIN

#define HEATER_0_PIN   38
#define HEATER_BED_PIN 41

#define FAN_PIN 10 //TODO DC vs PWM

//
// Misc. Functions
//

//TODO

//
// LCDs and Controllers
//

//TODO
