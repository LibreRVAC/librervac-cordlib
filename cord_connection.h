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
#include <stdint.h>
#include <stddef.h>

#ifdef __SDCC
#define CORD_INPUT_BUFFER_SIZE 100
#define CORD_OUTPUT_BUFFER_SIZE 150
#else
#define CORD_INPUT_BUFFER_SIZE 400
#define CORD_OUTPUT_BUFFER_SIZE 400
#endif

bool cord_connection_input_put(uint_fast8_t byte);
uint_fast8_t cord_connection_input_pull();
size_t cord_connection_input_size();

bool cord_connection_output_put(uint_fast8_t byte);
uint_fast8_t cord_connection_output_pull();
size_t cord_connection_output_size();

uint_fast32_t get_total_bytes_out();
uint_fast32_t get_total_bytes_in();
