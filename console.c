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

// standard includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

// simOS includes
#include "console.h"
#include "utils.h"



/* ====== Globals ====== */

size_t console_row;
size_t console_column;
uint8_t console_color;
uint16_t* console_buffer;



/* ====== PRIVATE console functions ====== */

static uint8_t __make_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

 
static uint16_t __make_vgaentry(char c, uint8_t color)
{
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | color16 << 8;
}

 
static void __ultoa(char *buf, unsigned long value)
{
    int i, ndigits = 0;

    // generate digits in reverse order
    do {
        buf[ndigits++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    // reverse digits to put them in the correct order
    for (i = 0; i < ndigits/2; ++i) {
        int swap_pos = ndigits - (i+1);
        char t = buf[i];
        buf[i] = buf[swap_pos];
        buf[swap_pos] = t;
    }

    // terminate the string
    buf[ndigits] = '\0';
}


static void __ltoa(char *buf, long value)
{
    unsigned raw;

    // handle negative values by converting to positive magnitude
    if (value < 0) {
        *buf++ = '-';
        raw = (unsigned) -value;
    } else {
        raw = (unsigned) value;
    }

    __ultoa(buf, raw);
}


static void __ltox(char *buf, long value)
{
    int i;
    for (i = 28; i >= 0; i -= 4) {
        *buf++ = "0123456789ABCDEF"[(value >> i) & 0xF];
    }
    *buf = '\0';
}


static void __scroll(void)
{
    int j;

    // move rows 1..VGA_NUMROWS-1 up one row
    for (j = 1; j < VGA_NUMROWS; j++) {
        memcpy(ROW_ADDR(j-1), ROW_ADDR(j), VGA_BYTES_PER_ROW);
    }

    // clear last row
    memset(ROW_ADDR(VGA_NUMROWS-1), '\0', VGA_BYTES_PER_ROW);
}



/* ====== PUBLIC console functions ====== */

void console__initialize()
{
    console_row = 0;
    console_column = 0;
    console_color = __make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    console_buffer = (uint16_t*) 0xB8000;
    for ( size_t y = 0; y < VGA_NUMROWS; y++ )
    {
        for ( size_t x = 0; x < VGA_NUMCOLS; x++ )
        {
            const size_t index = y * VGA_NUMCOLS + x;
            console_buffer[index] = __make_vgaentry(' ', console_color);
        }
    }
}

 
void console__setcolor(uint8_t color)
{
    console_color = color;
}

 
void console__putentryat(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_NUMCOLS + x;
    console_buffer[index] = __make_vgaentry(c, color);
}

 
void console__putchar(char c)
{
    if (c == '\n') {
        console_column = 0;

        /* scroll? */
        if (console_row == VGA_NUMROWS - 1) 
        {
            __scroll();
        } 
        else 
        {
            console_row++;
        }

        return;
    }
    console__putentryat(c, console_color, console_column, console_row);
    if ( ++console_column == VGA_NUMCOLS )
    {
        console_column = 0;
        if (console_row == VGA_NUMROWS - 1)
        {
            __scroll();
        }
        else
        {
            console_row++;
        }
    }
}
 

void console__write(const char* data)
{
    size_t datalen = strlen(data);
    for ( size_t i = 0; i < datalen; i++ )
        console__putchar(data[i]);
}


void console__printf(const char *fmt, ...)
{
    va_list args;
    char buf[MAXDIGITS + 2];
    bool is_long;

    va_start(args, fmt);


//  <TODO> START TO BE ATOMIC </TODO>

    while (*fmt != '\0') {
        switch (*fmt) {
        case '%':
            if (!*++fmt) goto done;

            if (*fmt == 'l') {
                is_long = true;
                if (!*++fmt) goto done;
            } else if (*fmt == 'p') {
                is_long = true;
            } else {
                is_long = false;
            }

            switch (*fmt) {
            case 'd':
                __ltoa(buf, is_long ? va_arg(args, long) : va_arg(args, int));
                console__write(buf);
                break;

            case 'u':
                __ultoa(buf, is_long ? va_arg(args, unsigned long) : va_arg(args, unsigned));
                console__write(buf);
                break;

            case 'x':
            case 'p':
                __ltox(buf, is_long ? va_arg(args, long) : va_arg(args, int));
                console__write(buf);
                break;

            case 'c':
                console__putchar(va_arg(args, int) & 0xFF);
                break;

            case 's':
                console__write(va_arg(args, const char *));
                break;

            default:
                console__putchar(*fmt);
                break;
            }
            break;

        default:
            console__putchar(*fmt);
            break;
        }
        ++fmt;
    }

done:
//  <TODO> STOP TO BE ATOMIC </TODO>

    va_end(args);
}
