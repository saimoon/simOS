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
#include "kassert.h"
#include "int_vectors.h"
#include "int.h"
#include "kbd.h"



/* ====== Globals ====== */

uint8_t kbd_us[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',     /* 9 */
    '9', '0', '-', '=', '\b',                           /* Backspace */
    '\t',                                               /* Tab */
    'q', 'w', 'e', 'r',                                 /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',       /* Enter key */
    0,                                                  /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   /* 39 */
    '\'', '`',   0,                                     /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',                 /* 49 */
    'm', ',', '.', '/',   0,                            /* Right shift */
    '*',
    0,                                                  /* Alt */
    ' ',                                                /* Space bar */
    0,                                                  /* Caps lock */
    0,                                                  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,                                                  /* < ... F10 */
    0,                                                  /* 69 - Num lock*/
    0,                                                  /* Scroll Lock */
    0,                                                  /* Home key */
    0,                                                  /* Up Arrow */
    0,                                                  /* Page Up */
    '-',
    0,                                                  /* Left Arrow */
    0,
    0,                                                  /* Right Arrow */
    '+',
    0,                                                  /* 79 - End key*/
    0,                                                  /* Down Arrow */
    0,                                                  /* Page Down */
    0,                                                  /* Insert Key */
    0,                                                  /* Delete Key */
    0,   0,   0,
    0,                                                  /* F11 Key */
    0,                                                  /* F12 Key */
    0,                                                  /* All other keys are undefined */
};  


/* ====== IRQ handler functions ====== */

// keyboard interrupt handler
void isr_kbd(uint8_t irq, uint32_t *regs)
{
    uint8_t ch = inb(0x60);
    if ( !(ch & 0x80) )
        console__printf("Key: %c\n", kbd_us[ch]);
    
    return;
}


/* ====== PRIVATE kbd functions ====== */

// enable keyboard
static void __enablekbd(void)
{
    int__irqdisable();
    while( ((inb(STATUS_PORT) & INPT_BUF_FULL)) );
    outb(KBD_ENABLE, KBD_CMD_BUF);
    int__irqenable();
}



/* ====== PUBLIC kbd functions ====== */

// Init keyboard
void kbd__init(void)
{
    // Enable keyboard
    __enablekbd();

    // Attach IRQ1 to the kbd interrupt handler
    int__irq_attach(IRQ1, (irqvfunc_t)isr_kbd);

    // Clear the output buffer
    inb(KBD_OUT_BUF);

  	// Enable IRQ1
  	int__enable_irq(IRQ1);
}
