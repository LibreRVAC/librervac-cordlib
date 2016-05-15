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
    BUTTON_HOME, ///< try to dock (MNBASH)
    BUTTON_CLEAN, ///< start cleaning
    BUTTON_SPOT, ///< clean one specific spot
    BUTTON_PAUSE, ///< clean one specific spot
    BUTTON_CONTINUE, ///< clean one specific spot
    BUTTON_ONE, ///< no defined meaning
    BUTTON_TWO, ///< no defined meaning
    BUTTON_THREE, ///< no defined meaning
} cord_button;

typedef enum {
    PRESSED, RELEASED,
} cord_button_state;

void cord_userbutton(cord_button button, cord_button_state state);
