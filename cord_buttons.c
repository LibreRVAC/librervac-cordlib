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

#include "cord_buttons.h"
#include "cord_cordlib.h"
#include "cord_errors.h"

void cord_userbutton(cord_button button, cord_button_state state) {
    switch (button) {
        case BUTTON_HOME:
            break;
        case BUTTON_CLEAN:
            break;
        case BUTTON_SPOT:
            break;
        case BUTTON_PAUSE:
            break;
        case BUTTON_CONTINUE:
            break;
        default:
            cord_error(ERROR_UNKNOWN_BUTTON);
            break;
    }
}
