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

#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#include "utils.h"



/* ====== PUBLIC utils functions ====== */
 
void memset(void *buf, uint8_t val, size_t len)
{
    uint8_t *p = buf;

    while (len > 0) {
        *p++ = val;
        len--;
    }
}

void memcpy(void *dst, const void *src, size_t len)
{
    uint8_t *d = dst;
    const uint8_t *s = src;

    while (len > 0) {
        *d++ = *s++;
        len--;
    }
}

size_t strlen(const char* str)
{
    size_t ret = 0;
    while ( str[ret] != 0 )
        ret++;
    return ret;
}

inline uint8_t inb(uint16_t port)
{
        uint8_t value;
        asm volatile("inb %1, %0" : "=a" (value) : "dN" (port));
        return(value);
}

inline void outb(uint8_t value, uint16_t port)
{
        asm volatile("outb %0, %1" : : "a" (value), "dN" (port));
}
