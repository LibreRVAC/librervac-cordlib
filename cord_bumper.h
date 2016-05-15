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

/// should be sent before sending any other more precise information
void cord_event_bumper_unknown();

/// On some hardware you can detect the position to some degree of precision.
/// ⟨uncertainty⟩ is a radius of collision cloud.
/// Coordinates are relative to the center of the robot.
void cord_event_bumper_xy(float x, float y, float uncertainty);

/// Round robots can use this. Angles are in radians, 0 means top
void cord_event_bumper(float angle, float uncertainty);

//void sensor // TODO
