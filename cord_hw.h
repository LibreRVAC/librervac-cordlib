/*
 * This file is part of the LibreRVAC project
 *
 * Copyright © 2015-2016
 *     Aleks-Daniel Jakimenko-Aleksejev <alex.jakimenko@gmail.com>
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
 */

#pragma once

#include "cord_motors.h"
#include <stdint.h>
#include <stdbool.h>

void set_callback_setup(void (*callback)(void));
void hw_setup();

void set_callback_step(void (*callback)(void));
void hw_step();

void set_callback_toggle_interrupts(void (*callback)(bool));
void hw_toggle_interrupts(bool enable);

void set_callback_process_beeps(void (*callback)());
void hw_process_beeps();

void set_callback_set_motor_throttle(void (*callback)(cord_motor_type, float));
void hw_set_motor_throttle(cord_motor_type motor, float throttle);

void set_callback_get_time(float (*callback)(void));
float hw_get_time(); // TODO floats do not have enough precision

// this is less universal but obviously it is much faster
void set_callback_get_time_ms(uint_fast32_t(*callback)(void));
uint_fast32_t hw_get_time_ms();

void set_callback_get_battery_voltage(float (*callback)(void));
float hw_get_battery_voltage();

void set_callback_get_battery_status(float (*callback)(void));
float hw_get_battery_status();

void set_callback_get_battery_current(float (*callback)(void));
float hw_get_battery_current();

void set_callback_get_battery_temperature(float (*callback)(void));
float hw_get_battery_temperature();

void set_callback_get_version(const char* (*callback)(void));
const char* hw_get_version();
