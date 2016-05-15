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

#include <stdlib.h>

#include "cord_wheels.h"
#include "cord_motors.h"
#include "cord_hw.h"
#include "cord_protocol.h"

static float speed_current;
static float speed_required;
static float speed_goal;

//static float distance_traveled; // total
//static float distance_required; // constantly increasing

static bool moving;
static uint_fast32_t end;

void cord_wheels_step() {
    if (moving && hw_get_milliseconds() >= end) {
        cord_stop_moving();
        cord_write_packet(60, JSON({"e":"move-finished"}));
    }
}

/* High-level */
float cord_get_speed_current() {
    return speed_current;
}

void cord_set_speed_goal(float speed) {
    speed_goal = speed;
    if (speed == 0) { // TODO temp until encoders are processed
        cord_set_motor_throttle(WHEEL_LEFT, 0);
        cord_set_motor_throttle(WHEEL_RIGHT, 0);
    } else {
        cord_set_motor_throttle(WHEEL_LEFT, 1);
        cord_set_motor_throttle(WHEEL_RIGHT, 1);
    }
}

/* Internal */
void cord_wheel_encoder(cord_motor_type motor) {
    // TODO do it properly
    if (speed_current < speed_required) {
        cord_set_motor_throttle(WHEEL_LEFT, 1);
        cord_set_motor_throttle(WHEEL_RIGHT, 1);
    } else if (speed_current > speed_required) {
        cord_set_motor_throttle(WHEEL_LEFT, 0);
        cord_set_motor_throttle(WHEEL_RIGHT, 0);
    }
}

void cord_overcurrent(cord_motor_type motor) {
    // TODO do it properly
    switch (motor) {
        case WHEEL_RIGHT:
        case WHEEL_LEFT:
            cord_set_motor_throttle(WHEEL_LEFT, 0);
            cord_set_motor_throttle(WHEEL_RIGHT, 0);
            break;
        case VACUUM:
            cord_set_motor_throttle(VACUUM, 0);
            break;
        default:
            // TODO error
            break;
    }
}

/* External */

// TODO current implementation is a placeholder

// TODO proper system will probably require a PID

void cord_stop_moving() {
    hw_set_motor_throttle(WHEEL_LEFT, 0);
    hw_set_motor_throttle(WHEEL_RIGHT, 0);
    moving = false;
}

void cord_move(float length) {
    if (length > 0) {
        hw_set_motor_throttle(WHEEL_LEFT,  +0.5);
        hw_set_motor_throttle(WHEEL_RIGHT, +0.5);
    } else {
        hw_set_motor_throttle(WHEEL_LEFT,  -0.5);
        hw_set_motor_throttle(WHEEL_RIGHT, -0.5);
    }
    moving = true;
    end = hw_get_milliseconds() + 200;
    //distance_required += length;
}

void cord_move_arc(float length, float radius) {
    // TODO get rid of placeholders
    if (length < 0.01) {
        if (radius > 0) {
            hw_set_motor_throttle(WHEEL_LEFT,  -0.5);
            hw_set_motor_throttle(WHEEL_RIGHT, +0.5);
        } else {
            hw_set_motor_throttle(WHEEL_LEFT,  +0.5);
            hw_set_motor_throttle(WHEEL_RIGHT, -0.5);
        }
    }
    moving = true;
    end = hw_get_milliseconds() + 700 + (rand() % 500);
    //distance_required += length;
    // TODO arc
}
