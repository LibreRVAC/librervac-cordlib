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

typedef enum {
    WHEEL_LEFT = 0, ///< low-level, avoid using it
    WHEEL_RIGHT, ///< low-level, avoid using it
    VACUUM, ///< vacuum motor
    BRUSH_MAIN, ///< main brush
    BRUSH_LEFT, ///< separate control of side brushes is MNBASH
    BRUSH_RIGHT, ///< separate control of side brushes is MNBASH
    BRUSH_SIDES, ///< control both side brushes at the same time
    QUASI_MOTOR_LAST, ///< used to determine enum boundaries
} cord_motor_type;

void cord_event_encoder(cord_motor_type motor);
void cord_event_overcurrent(cord_motor_type motor);

void cord_set_motor_throttle(cord_motor_type motor, float throttle);
float cord_get_motor_throttle(cord_motor_type motor);
