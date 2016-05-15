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

#define CORDLIB_VERSION "0.0.1"

// TODO

typedef enum {
    MODE_BOOTING, ///< before we receive anything from linux board
    MODE_IDLE, ///< waiting for commands
    MODE_WORKING, ///< start cleaning
    MODE_SLEEPING, ///< low-energy mode (MNBASH)
    MODE_DOCKING, ///< go to docking station (MNBASH)
} cord_working_mode;

extern cord_working_mode cord_mode;
