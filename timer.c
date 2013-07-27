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

// simOS includes
#include "utils.h"
#include "kassert.h"
#include "int_vectors.h"
#include "int.h"
#include "timer.h"



/* Programmable interval timer (PIT)
 *
 *   Fpit = Fin / divisor
 *   divisor = Fin / Fpit
 *
 * Where:
 *
 *   Fpit = The desired interrupt frequency
 *   Fin  = PIT input frequency
 *
 * The desired timer interrupt frequency is provided by the definition CLK_TICK.
 * CLK_TICK defines the desired number of system clock ticks per second.
 * That value is a user configurable setting that defaults to 100 (100 ticks per second = 10 MS interval).
 */
#define PIT_DIVISOR  ((uint32_t)PIT_CLOCK/(uint32_t)CLK_TICK)



/* ====== IRQ handler functions ====== */

// timer interrupt handler
void isr_timer(uint8_t irq, uint32_t *regs)
{
//    console__printf("tick\n");

//    <TODO/>  
}



/* ====== PUBLIC timer functions ====== */

void timer__init(void)
{
    // uint32_t to avoid compile time overflow errors
    uint32_t divisor = PIT_DIVISOR;
    KASSERT(divisor <= 0xffff);

    // Attach IRQ0 to the timer interrupt handler
    int__irq_attach(IRQ0, (irqvfunc_t)isr_timer);

    // Send the command byte to configure counter 0
    outb(PIT_OCW_MODE_SQUARE|PIT_OCW_RL_DATA|PIT_OCW_COUNTER_0, PIT_REG_COMMAND);

    // Set the PIT input frequency divisor
    outb((uint8_t)(divisor & 0xff),  PIT_REG_COUNTER0);
    outb((uint8_t)((divisor >> 8) & 0xff), PIT_REG_COUNTER0);

    // And enable IRQ0
    int__enable_irq(IRQ0);
}
