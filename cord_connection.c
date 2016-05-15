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

#include "cord_connection.h"
#include "cord_hw.h"

#include <stddef.h>
#include <stdint.h>
#define dir       bool
#define RECEIVING false
#define SENDING   true

// TODO Currently we may be pushing to this buffer from different ISRs at
// the same time, which under certain circumstances will lead to data
// corruption. This is not right at all. Instead, we should be allocating some
// space beforehand, and then filling that space without any disturbance.

// TODO Consider doing something to save ourselves
// from ring buffer copy-pasting (DONE! Now use it here!)
volatile uint_fast8_t cord_in[CORD_INPUT_BUFFER_SIZE]; ///< Typical ring buffer
volatile uint_fast8_t cord_out[CORD_OUTPUT_BUFFER_SIZE]; ///< Typical ring buffer
volatile uint_fast16_t in_head;  ///< pulling from here
volatile uint_fast16_t in_empty; ///< pushing here
volatile uint_fast16_t in_size;  // makes our life slightly easier
volatile uint_fast16_t out_head;  ///< pulling from here
volatile uint_fast16_t out_empty; ///< pushing here
volatile uint_fast16_t out_size;  // makes our life slightly easier

volatile uint_fast32_t total_bytes_out;
volatile uint_fast32_t total_bytes_in;


// TODO reverse full-duplex confirmation
//bool initialized;
//bool full_duplex;
//dir  direction = RECEIVING; // only used for half-duplex communication

//volatile uint_fast8_t in_ticker;
//volatile uint_fast8_t out_ticker;

// The protocol is designed for full-duplex communication.
// If hardware is limited to half-duplex then we just have to be
// quiet when listening.


// INPUT

//#include "../gpio.h"

bool cord_connection_input_put(uint_fast8_t byte) {
    /*if (!initialized) {
        /f (byte == 0xFD)
            full_duplex = true;
        else if (byte == 0x0D) {
            full_duplex = false;
        } else
            return false; // initial byte is incorrect… this is a catastrophe (XXX)
        initialized = true;
        return true;
    }*/
    //if (!full_duplex && in_ticker++ % 2 == 0)
    //    return true; // TODO here we can probably assert that byte == 0xFF

    if (in_size >= CORD_INPUT_BUFFER_SIZE) // whoops, no space left!
        return false; // TODO
    cord_in[in_empty] = byte;
    in_size++;
    if (++in_empty >= CORD_INPUT_BUFFER_SIZE)
        in_empty = 0;
    total_bytes_in++;
    return true;
}

uint_fast8_t cord_connection_input_pull() {
    uint_fast8_t byte;
    if (in_size == 0)
        return 0; // no more data
    byte = cord_in[in_head++];
    in_size--;
    if (in_head >= CORD_INPUT_BUFFER_SIZE)
        in_head = 0;
    return byte;
}

size_t cord_connection_input_size() {
    return in_size;
}

// OUTPUT

bool cord_connection_output_put(uint_fast8_t byte) {
    if (out_size >= CORD_OUTPUT_BUFFER_SIZE) // whoops, no space left!
        return false; // TODO
    cord_out[out_empty] = byte;
    out_size++;
    if (++out_empty >= CORD_OUTPUT_BUFFER_SIZE)
        out_empty = 0;
    return true;
}

uint_fast8_t cord_connection_output_pull() {
    uint_fast8_t byte;
    //if (!initialized)
    //return 0; // TODO 0 or 0xFF?
    //if (!full_duplex && out_ticker++ % 2 == 1)
    //return 0; // it is not our turn
    if (out_size == 0)
        return 0; // no more data
    byte = cord_out[out_head++];
    out_size--;
    if (out_head >= CORD_OUTPUT_BUFFER_SIZE)
        out_head = 0;
    // TODO bug somewhere. It looks like we're getting here when the size is
    // actually 0
    total_bytes_out++;
    return byte;
}

size_t cord_connection_output_size() {
    return out_size;
}

// TODO on 8-bit systems this will probably result in garbage

uint_fast32_t get_total_bytes_out() {
    return total_bytes_out;
}

uint_fast32_t get_total_bytes_in() {
    return total_bytes_in;
}
