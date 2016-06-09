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
#include "../jsmn/jsmn.h" // TODO required but not included in the source
#include "cord_sound.h"
#include "cord_wheels.h"
#include "cord_motors.h"
#include "cord_hw.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef __SDCC
#define PROTOCOL_BUFFER_SIZE 200
#define PROTOCOL_TOKENS 30
#else
#define PROTOCOL_BUFFER_SIZE 100
#define PROTOCOL_TOKENS 20
#define MAX_PACKET_SIZE 150
#endif
//#define VOID_BYTE       0x00

// TODO we can get rid of this buffer to save some space
// + 1 for extra null byte
char buf[PROTOCOL_BUFFER_SIZE + 1]; // max packet size // TODO increase it?
size_t buf_size;

// We will be using these global vars. Makes life easier.
jsmn_parser parser;
jsmntok_t tokens[PROTOCOL_TOKENS]; // Also specifies the max amount of tokens
int token_count;

static inline bool eq(size_t index, const char *str) {
    return strncmp(str, buf + tokens[index].start,
            tokens[index].end - tokens[index].start) == 0;
}

// TODO is there any way to parse it in one pass?

// TODO we can optimize things greatly if we check just the
// first character (but not all clients are going to send ordered hashes)
// TODO or perhaps we should consider other protocol (i.e. non plaintext)

static void process_beep() {
#ifndef __SDCC
    float pitch    = 0;
    float duration = 0;
    float volume   = 0;
    size_t skip_to = 0;
    size_t i;
    for (i = 1; i < token_count; i++) { // TODO increment by two?
        if (tokens[i].start < skip_to)
            continue;
        if (tokens[i].type == JSMN_OBJECT || tokens[i].type == JSMN_ARRAY) {
            skip_to = tokens[i].end;
        } else if (tokens[i].type == JSMN_STRING && i + 1 < token_count) {
            if      (eq(i, "pitch"   ))
                pitch    = atof(buf + tokens[i + 1].start);
            else if (eq(i, "duration"))
                duration = atof(buf + tokens[i + 1].start);
            else if (eq(i, "volume"  ))
                volume   = atof(buf + tokens[i + 1].start);
        }
        // we will just skip JSMN_PRIMITIVE and possibly something else
    }
    cord_buzzer_queue_beep(pitch, duration, volume);
#endif
}

static void process_motor() {
#ifndef __SDCC
    bool ok = false;
    cord_motor_type motor;
    float throttle = 0;
    size_t skip_to = 0;
    size_t i;
    for (i = 1; i < token_count; i++) { // TODO increment by two?
        if (tokens[i].start < skip_to)
            continue;
        //if (tokens[i].type == JSMN_OBJECT || tokens[i].type == JSMN_ARRAY) {
        if (tokens[i].type == JSMN_STRING && i + 1 < token_count) {
            skip_to = tokens[i + 1].end;
            if (eq(i, "motor")) {
                char *str = buf + tokens[i + 1].start;
                int len = tokens[i + 1].end - tokens[i + 1].start;
                ok = true;
                if      (strncmp("wheel left",  str, len) == 0)
                    motor = WHEEL_LEFT;
                else if (strncmp("wheel right", str, len) == 0)
                    motor = WHEEL_RIGHT;
                else if (strncmp("brush main",  str, len) == 0) {
                    motor = BRUSH_MAIN;
                } else if (strncmp("brush sides", str, len) == 0)
                    motor = BRUSH_SIDES;
                else if (strncmp("brush left",  str, len) == 0)
                    motor = BRUSH_LEFT;
                else if (strncmp("brush right", str, len) == 0)
                    motor = BRUSH_RIGHT;
                else if (strncmp("vacuum",      str, len) == 0)
                    motor = VACUUM;
                else
                    ok = false;
            } else if (eq(i, "throttle"))
                throttle = atof(buf + tokens[i + 1].start);
        }
        // we will just skip JSMN_PRIMITIVE and possibly something else
    }
    if (ok) // TODO handle broken packets
        cord_set_motor_throttle(motor, throttle);
#endif
}

static void process_move() {
    float length = 0;
    float radius = 0;
    size_t skip_to = 0;
    size_t i;
    for (i = 1; i < token_count; i++) {
        if (tokens[i].start < skip_to)
            continue;
        if (tokens[i].type == JSMN_STRING && i + 1 < token_count) {
            skip_to = tokens[i + 1].end;
            if (eq(i, "length")) {
                length = atof(buf + tokens[i + 1].start);
            } else if (eq(i, "radius"))
                radius = atof(buf + tokens[i + 1].start);
        }
        // we will just skip JSMN_PRIMITIVE and possibly something else
    }
#ifndef __SDCC
    if (length != 0 && radius != 0) // TODO handle broken packets
        cord_move_arc(length, radius);
    else if (length != 0)
        cord_move(length);
#endif
}

static void process_command(const char *str, size_t length) {
    if (strncmp("motor", str, length) == 0)
        process_motor();
    else if (strncmp("move", str, length) == 0)
        process_move();
    else if (strncmp("beep", str, length) == 0)
        process_beep();
}

#include "cord_hw.h"
#include "cord_protocol.h"

// TODO make it bulletproof

static void process_packet() {
    size_t skip_to = 0;
    size_t i;
    jsmn_init(&parser);
    token_count = jsmn_parse(&parser, buf, buf_size, tokens, PROTOCOL_TOKENS);
    /*char buffer[40];
    sprintf(buffer, "Token count = %d\r\nJSON:", token_count);
    for (int i = 0; i < 100; i++) {
        if (buffer[i] == 0)
            break;
        cord_connection_output_put(buffer[i]);
    }
    //for (int i = 0; i < buf_size; i++) {
    //    cord_connection_output_put(buf[i]);
    //}
    cord_connection_output_put('\r');
    cord_connection_output_put('\n');
     */
    if (token_count < 0)
        return; // TODO handle corrupt data gracefully

    // TODO make use of non-strict mode, allow some sloppiness
    if (token_count < 1 || tokens[0].type != JSMN_OBJECT) // should be surrounded by {}
        return; // TODO handle weird packets gracefully

    for (i = 1; i < token_count; i++) {
        if (tokens[i].start < skip_to)
            continue;
        //if (tokens[i].type == JSMN_OBJECT || tokens[i].type == JSMN_ARRAY) {
        if (tokens[i].type == JSMN_STRING) {
            if (i + 1 < token_count) {
                if (eq(i, "c") && i + 1 < token_count) {
                    process_command(&buf[tokens[i + 1].start], tokens[i + 1].end - tokens[i + 1].start);
                    return;
                }
                skip_to = tokens[i + 1].end;
            }
        } else
            return; // TODO parsing error
    }
}

void cord_protocol_process_input() {
    char byte;
    // TODO should it loop until we have nothing to process?
    if (cord_connection_input_size() <= 0)
        return; // nothing to do here
    byte = cord_connection_input_pull();
    if (byte == SEP) {
        if (buf_size > 0)
            process_packet();
        buf_size = 0;
        return;
    }
    if (buf_size >= PROTOCOL_BUFFER_SIZE)
        return; // TODO handle overflow gracefully
    buf[buf_size++] = byte;
}

#ifdef __SDCC
// error 2: Initializer element is not constant
// Also has to be outside of a function, otherwise we are polluting the stack
char packet_buf[MAX_PACKET_SIZE];
#endif

void cord_write_packet(size_t max_packet_size, const char *format, ...) {
#ifndef __SDCC
    char packet_buf[max_packet_size];
#endif
    va_list args;
    int packet_length;
    int i;
    va_start(args, format);
#ifdef __SDCC
    // TODO vsnprintf should be supported with --std-c11
    packet_length = vsprintf(packet_buf, format, args);
#else
    packet_length = vsnprintf(packet_buf, max_packet_size, format, args);
#endif
    va_end(args);

    if (packet_length < 0 || packet_length >= max_packet_size)
        return; // TODO is there any better way to handle it?

    hw_toggle_interrupts(false);
    for (i = 0; i < packet_length; i++)
        cord_connection_output_put(packet_buf[i]);
    cord_connection_output_put(SEP);
    hw_toggle_interrupts(true);
}
