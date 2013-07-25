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

#ifndef SIMOS_KASSERT_H
#define SIMOS_KASSERT_H


#include "console.h"

#define HALT() while (1)

#define KASSERT(exp) \
do { \
        if (!(exp)) { \
                console__printf("Assertion " #exp " failed at %s, line %d", __FILE__, __LINE__); \
                HALT(); \
        } \
} while (0)


#endif /* SIMOS_KASSERT_H */
