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

#define RBUFFER(name, type, size1)         \
struct name ## _typedef {                   \
    volatile size_t size;                    \
    volatile size_t read_index;               \
    volatile size_t write_index;               \
    volatile type data[size1];                  \
};                                               \
struct name ## _typedef name = { .size = size1 }; \

#define RBUFFER_HAS_DATA(name)  \
  ( (name).read_index != (name).write_index )

#define RBUFFER_HAS_SPACE(name) \
  ( ((name).read_index + 1) % (name).size != (name).write_index )

#define RBUFFER_ELEMS(name) ( (name).size )

// you should call RBUFFER_HAS_SPACE before writing to prevent overruns

#define RBUFFER_WRITE(name, value) {          \
  (name).data[(name).write_index++] = (value); \
  (name).write_index %= (name).size;            \
}                                                \

#define RBUFFER_READ(name)                                    \
  ( (name).read_index + 1 < (name).size                        \
    ? (name).data[(name).read_index++]                          \
    : (name).data[((name).read_index = 0) ? 0 : (name).size - 1] \
  )                                                               \

