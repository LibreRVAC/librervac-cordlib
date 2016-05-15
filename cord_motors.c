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

#include "cord_motors.h"
#include "cord_hw.h"

float cord_motor_throttle[QUASI_MOTOR_LAST];

void cord_event_encoder(cord_motor_type motor) {
    switch (motor) {
        case WHEEL_LEFT:
            break;
        case WHEEL_RIGHT:
            break;
        default:
            // TODO
            break;
    }
}

void cord_event_overcurrent(cord_motor_type motor) {
    // TODO
}

void cord_set_motor_throttle(cord_motor_type motor, float throttle) {
    cord_motor_throttle[motor] = throttle;
    hw_set_motor_throttle(motor, throttle); // TODO … todo what?
}

float cord_get_motor_throttle(cord_motor_type motor) {
    return cord_motor_throttle[motor];
}
