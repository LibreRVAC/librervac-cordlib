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

#include <stdio.h>

#include "cord_init.h"
#include "cord_hw.h"
#include "cord_connection.h"
#include "cord_protocol.h"
#include "cord_sound.h"
#include "cord_cordlib.h"
#include "cord_wheels.h"

static uint_fast32_t steps_per_second = 0;

float cord_get_main_loop_frequency() {
    return steps_per_second;
}

// TODO all these can be optimized

// TODO lots of things are happening exactly at the same moment.

static void main_loop_frequency_step() {
    static uint_fast32_t cur_steps_per_second = 0;
    static uint_fast32_t then = 0;
    uint_fast32_t now = hw_get_time_ms();
    if (now % 1000 == 0 && now != then) {
        steps_per_second = cur_steps_per_second;
        cur_steps_per_second = 0;
        then = now;
    }
    cur_steps_per_second++;
}

static void bandwidth_step() {
    static uint_fast32_t then = 0;
    static uint_fast32_t total_bytes_in_then = 0;
    static uint_fast32_t total_bytes_out_then = 0;
#ifndef __SDCC
    uint_fast32_t now = hw_get_time_ms();
#else
    // this function is overloaded with 32-bit ints, so we have
    // to preallocate some
    static uint_fast32_t now;
    now = hw_get_time_ms();
    // TODO we are wasting 4 bytes of RAM because of this
#endif
    if (now % 1000 == 0 && now != then) {
        uint_fast32_t total_bytes_in_now = get_total_bytes_in();
        uint_fast32_t total_bytes_out_now = get_total_bytes_out();

        uint_fast32_t in_diff  = total_bytes_in_now  - total_bytes_in_then;
        uint_fast32_t out_diff = total_bytes_out_now - total_bytes_out_then;

        cord_write_packet(100, JSON({"c":"info","bandwidth-in":%d,"bandwidth-out":%d})
        , in_diff, out_diff);
        then = now;
        total_bytes_in_then = total_bytes_in_now;
        total_bytes_out_then = total_bytes_out_now;
    }
}

static void alive_beep_step() {
    static uint_fast32_t then = 0;
    uint_fast32_t now = hw_get_time_ms();
    if (now % 15000 == 0 && now != then) {
        //cord_buzzer_queue_beep(500, 0.02, 0.2);

        then = now;
    }
}

static void send_general_info() {
    // TODO send this only when required
    static uint_fast32_t then = 0;
    uint_fast32_t now = hw_get_time_ms();
    if (now % 1250 == 0 && now != then) { // TODO obviously this is too often
        cord_write_packet(100, JSON({"c":"info","firmware-version":"%s","cordlib-version":"%s"})
        , hw_get_version(), CORDLIB_VERSION);
        then = now;
    }
}

static void send_info_step() {
    static uint_fast32_t then = 0;
    uint_fast32_t now = hw_get_time_ms();
    if (now % 50 == 0 && now != then) {
        switch ((now / 50) % 5) {
            case 0:
                cord_write_packet(70, JSON({"c":"info","main-steps":%d}), steps_per_second);
                break;
#ifndef __SDCC
            case 1:
                cord_write_packet(70, JSON({"c":"info","battery-status":%f}), (double) hw_get_battery_status());
                break;
            case 2:
                cord_write_packet(70, JSON({"c":"info","battery-voltage":"%f"}), (double) hw_get_battery_voltage());
                break;
            case 3:
                cord_write_packet(70, JSON({"c":"info","battery-current":%f}), (double) hw_get_battery_current());
                break;
            case 4:
                cord_write_packet(70, JSON({"c":"info","battery-temperature":"%f"}), (double) hw_get_battery_temperature());
                break;
#endif
        }
        then = now;
    }
}

void cord_event_init() {
    hw_setup();
    //cord_buzzer_queue_beep(1000, 0.1, 0.2);
#ifdef TESTING
    int count = 0;
#endif
    while (1) {
#ifndef __SDCC
        cord_protocol_process_input();
        cord_wheels_step();
#endif
        hw_step();
        main_loop_frequency_step();
#ifndef __SDCC
        bandwidth_step();
        alive_beep_step();
        send_general_info();
#endif
        send_info_step();
#ifdef TESTING
        // The only reason to exit from this infinite loop is if we are running tests
        if (count++ > 10)
            break;
#endif
    }
}
