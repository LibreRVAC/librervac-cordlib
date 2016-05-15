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

#include "cord_sound.h"
#include "cord_hw.h"

#include <stddef.h>
#include <stdint.h>

Beep beeps[BUZZER_BUFFER_SIZE]; ///< Typical ring buffer
uint_fast16_t head;  ///< pulling from here
uint_fast16_t empty; ///< pushing here
uint_fast16_t size;  // makes our life slightly easier

bool cord_buzzer_queue_beep(float frequency, float duration, float volume) {
    if (size >= BUZZER_BUFFER_SIZE) // whoops, no space left!
        return false;
    beeps[empty].frequency = frequency;
    beeps[empty].duration = duration;
    beeps[empty].volume = volume;
    size++;
    if (++empty >= BUZZER_BUFFER_SIZE)
        empty = 0;
    hw_process_beeps(); // inform the implementation that there are new values
    return true;
}

Beep* cord_buzzer_get_beep() {
    Beep* beep;
    if (size == 0)
        return NULL; // no more beeps
    beep = &beeps[head++];
    size--;
    if (head >= BUZZER_BUFFER_SIZE)
        head = 0;
    return beep;
    // TODO The data may change while you are looking at it. But that's ok,
    // the implementation is not supposed to hold more than one or two beeps.
}
