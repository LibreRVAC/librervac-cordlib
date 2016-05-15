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

#include <stdbool.h>

// TODO support for speakers

#define BUZZER_BUFFER_SIZE 200 // TODO

typedef struct Beep {
    float frequency;
    float duration;
    // TODO do not even include it if it is not supported
    float volume; /**< MNBASH */
} Beep;

bool cord_buzzer_queue_beep(float frequency, float duration, float volume);
Beep* cord_buzzer_get_beep();
