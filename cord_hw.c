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

#include "cord_hw.h"

// TODO This is getting annoying. Write a macro?

// TODO We should probably get rid of these if statements. That is, instead
// of checking if the value is pointing to a function we should default to
// some dummy function that does nothing. This way we will get rid of
// redundant checks in hot paths without sacrificing anything because
// most of these callbacks are going to be set (and if they're not set, then
// why call them in hot paths?)

// hw_setup
void (*callback_setup)(void) = 0;

void set_callback_setup(void (*callback)(void) ) {
    callback_setup = callback;
}

void hw_setup() {
    if (callback_setup == 0)
        return;
    callback_setup();
}

// hw_step
void (*callback_step)(void) = 0;

void set_callback_step(void (*callback)(void) ) {
    callback_step = callback;
}

void hw_step() {
    if (callback_step == 0)
        return;
    callback_step();
}

// hw_toggle_interrupts
void (*callback_toggle_interrupts)(bool) __reentrant = 0;

void set_callback_toggle_interrupts(void (*callback)(bool) ) {
    callback_toggle_interrupts = callback;
}

void hw_toggle_interrupts(bool enable) {
    if (callback_toggle_interrupts == 0)
        return;
    callback_toggle_interrupts(enable);
}

// hw_beep
void (*callback_process_beeps)() = 0;

void set_callback_process_beeps(void (*callback)()) {
    callback_process_beeps = callback;
}

void hw_process_beeps() {
    if (callback_process_beeps == 0)
        return;
    callback_process_beeps();
}

// hw_set_motor_throttle
void (*callback_set_motor_throttle)(cord_motor_type, float) __reentrant = 0;

void set_callback_set_motor_throttle(void (*callback)(cord_motor_type, float) ) {
    callback_set_motor_throttle = callback;
}

void hw_set_motor_throttle(cord_motor_type motor, float throttle) {
    if (callback_set_motor_throttle == 0)
        return;
    callback_set_motor_throttle(motor, throttle);
}

// hw_get_time
float (*callback_get_time)(void) = 0;

void set_callback_get_time(float (*callback)(void) ) {
    callback_get_time = callback;
}

float hw_get_time() {
    if (callback_get_time == 0)
        return 0.0;
    return callback_get_time();
}

// hw_get_milliseconds
uint_fast32_t (*callback_get_milliseconds)(void) = 0;

void set_callback_get_milliseconds(uint_fast32_t (*callback)(void) ) {
    callback_get_milliseconds = callback;
}

uint_fast32_t hw_get_milliseconds() {
    if (callback_get_milliseconds == 0)
        return 0;
    return callback_get_milliseconds();
}

// hw_get_battery_voltage
float (*callback_get_battery_voltage)(void) = 0;

void set_callback_get_battery_voltage(float (*callback)(void) ) {
    callback_get_battery_voltage = callback;
}

float hw_get_battery_voltage() {
    if (callback_get_battery_voltage == 0)
        return 0.0;
    return callback_get_battery_voltage();
}

// hw_get_battery_status
float (*callback_get_battery_status)(void) = 0;

void set_callback_get_battery_status(float (*callback)(void) ) {
    callback_get_battery_status = callback;
}

float hw_get_battery_status() {
    if (callback_get_battery_status == 0)
        return 0.0;
    return callback_get_battery_status();
}

// hw_get_battery_current
float (*callback_get_battery_current)(void) = 0;

void set_callback_get_battery_current(float (*callback)(void) ) {
    callback_get_battery_current = callback;
}

float hw_get_battery_current() {
    if (callback_get_battery_current == 0)
        return 0.0;
    return callback_get_battery_current();
}

// hw_get_battery_temperature
float (*callback_get_battery_temperature)(void) = 0;

void set_callback_get_battery_temperature(float (*callback)(void) ) {
    callback_get_battery_temperature = callback;
}

float hw_get_battery_temperature() {
    if (callback_get_battery_temperature == 0)
        return 0.0;
    return callback_get_battery_temperature();
}

// hw_get_version
const char* (*callback_get_version)(void) = 0;

void set_callback_get_version(const char* (*callback)(void) ) {
    callback_get_version = callback;
}

const char* hw_get_version() {
    if (callback_get_version == 0)
        return "N/A";
    return callback_get_version();
}
