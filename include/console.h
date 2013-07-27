/*
 * Copyright (C) 2013 - Simone Rotondo - http://www.piemontewireless.net/
 * simOS - tiny x86 kernel
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

#ifndef SIMOS_CONSOLE_H
#define SIMOS_CONSOLE_H

// standard includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



// VGA defines
#define VGA_VIDEOMEM 0xB8000
#define VGA_NUMCOLS 80
#define VGA_NUMROWS 25
#define VGA_BYTES_PER_ROW (VGA_NUMCOLS * 2)
#define MAXDIGITS 39
#define ROW_ADDR(row) (((uint8_t *) VGA_VIDEOMEM) + ((row) * VGA_BYTES_PER_ROW))

// Hardware text mode color constants
enum vga_color
{
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};



/* PUBLIC console functions */
void console__initialize();
void console__setcolor(uint8_t color);
void console__putentryat(char c, uint8_t color, size_t x, size_t y);
void console__putchar(char c);
void console__write(const char* data);
void console__printf(const char *fmt, ...);


#endif /* SIMOS_CONSOLE_H */
