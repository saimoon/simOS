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

#ifndef SIMOS_UTILS_H
#define SIMOS_UTILS_H

// standard includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



// utils defines
#define hlt() __asm__("hlt");



/* PUBLIC utils functions */
void memset(void *buf, uint8_t val, size_t len);
void memcpy(void *dst, const void *src, size_t len);
size_t strlen(const char* str);
uint8_t inb(uint16_t port);
void outb(uint8_t value, uint16_t port);


#endif /* SIMOS_UTILS_H */
