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

#include "cord_bumper.h"
#include "cord_motors.h"
#include "cord_protocol.h"

void cord_event_bumper_unknown() {
    //cord_set_speed_goal(0);
    // TODO report to linux board
}

//void cord_event_bumper_xy(float x, float y, float uncertainty) {
    //cord_set_speed_goal(0);
    // TODO report to linux board
//}

void cord_event_bumper(float angle, float uncertainty) {
    cord_write_packet(60, JSON({"e":"bump","angle":%f,"+-":%f}), angle, uncertainty);
    //cord_set_speed_goal(0); // TODO
}
