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

#ifndef SIMOS_TIMER_H
#define SIMOS_TIMER_H

// standard includes
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



/* Programmable interval timer (PIT) model 8253 or 8254 */
#define PIT_CLOCK   1193180         // PIT input clock
#define CLK_TICK    (100)           // Clock tick of the system (frequency Hz)


/* Programmable Interrupt Timer Definitions */
#define PIT_REG_COUNTER0        0x40
#define PIT_REG_COUNTER1        0x41
#define PIT_REG_COUNTER2        0x42
#define PIT_REG_COMMAND         0x43


/* PIT command bit defintions */
#define PIT_OCW_BINCOUNT_BCD    (1 << 0)                    // vs binary
#define PIT_OCW_MODE_SHIFT      (1)
#define PIT_OCW_MODE_MASK       (7 << PIT_OCW_MODE_SHIFT)
#define PIT_OCW_MODE_TMCNT      (0 << PIT_OCW_MODE_SHIFT)   // Terminal count
#define PIT_OCW_MODE_ONESHOT    (1 << PIT_OCW_MODE_SHIFT)   // One shot
#define PIT_OCW_MODE_RATEGEN    (2 << PIT_OCW_MODE_SHIFT)   // Rate gen
#define PIT_OCW_MODE_SQUARE     (3 << PIT_OCW_MODE_SHIFT)   // Square wave generation
#define PIT_OCW_MODE_SWTRIG     (4 << PIT_OCW_MODE_SHIFT)   // Software trigger
#define PIT_OCW_MODE_HWTRIG     (5 << PIT_OCW_MODE_SHIFT)   // Hardware trigger
#define PIT_OCW_RL_SHIFT        (4)
#define PIT_OCW_RL_MASK         (3 << PIT_OCW_RL_SHIFT)
#define PIT_OCW_RL_LATCH        (0 << PIT_OCW_RL_SHIFT)
#define PIT_OCW_RL_LSBONLY      (1 << PIT_OCW_RL_SHIFT)
#define PIT_OCW_RL_MSBONLY      (2 << PIT_OCW_RL_SHIFT)
#define PIT_OCW_RL_DATA         (3 << PIT_OCW_RL_SHIFT)
#define PIT_OCW_COUNTER_SHIFT   (6)
#define PIT_OCW_COUNTER_MASK    (3 << PIT_OCW_COUNTER_SHIFT)
#define PIT_OCW_COUNTER_0       (0 << PIT_OCW_COUNTER_SHIFT)
#define PIT_OCW_COUNTER_1       (1 << PIT_OCW_COUNTER_SHIFT)
#define PIT_OCW_COUNTER_2       (2 << PIT_OCW_COUNTER_SHIFT)



/* PUBLIC int functions */
void timer__init(void);


#endif /* SIMOS_INT_H */
