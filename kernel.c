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

// Check if the compiler thinks if we are targeting the wrong operating system
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

// simOS includes
#include "console.h"
#include "kassert.h"
#include "utils.h"
#include "multiboot.h"
#include "mem.h"
#include "int_vectors.h"
#include "int.h"
#include "timer.h"
#include "kbd.h"

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif



/* ====== PUBLIC kernel functions ====== */

void kernel_main(uint32_t magic, uint32_t multiboot_info_addr)
{
    extern uint32_t __BSS_START;

    KASSERT(magic == MULTIBOOT_BOOTLOADER_MAGIC);

    // Init BSS section
    mem__bss_init();
    KASSERT(*((uint8_t *)&__BSS_START+4) == '\0');

    // Init Console
    console__initialize();
    console__write("Hello, I'm simOS.\nReady...\n\n");

/*
    // Dump memory map
    mem__dump_map();
*/

/*    
    // Dump Multiboot info
    multiboot__dump_info(magic, multiboot_info_addr);
*/

    // Init GDT
    mem__gdt_init();
    console__printf("* Init GDT\n");
    uint32_t cs, ds, ss;
    asm (
         "mov %%cs, %[x1] ;"
         "mov %%ds, %[x2] ;"
         "mov %%ss, %[x3] ;"
         : [x1] "=m" (cs), [x2] "=m" (ds), [x3] "=m" (ss)
    );
    KASSERT(cs == KERNEL_CS);
    KASSERT(ds == KERNEL_DS);
    KASSERT(ss == KERNEL_DS);

    // Init Paging
    mem__paging_init(multiboot_info_addr);
    console__printf("* Memory Paging Enabled\n");

    // Init IDT
    int__idt_init();
    console__printf("* Init Interrupts\n");

    // attach page fault irq handler
    mem__pagefaultirq();

    // Init TIMER
    timer__init();
    console__printf("* Init Timer\n");
    
    // Init KBD
    kbd__init();
    console__printf("* Init Keyboard\n");

    // enable interrupts
    int__irqenable();
    console__printf("* Enable Interrupts\n");

/*
    // test interrupts 
    __asm__ ("int $34");
*/
    // test interrupts
//    void (*x)(void) = 0x00000000;             // ISR6: invalid opcode
    void (*x)(void) = (void *)0xFFFF0000;       // ISR14: page fault
    x();

    // Infinite loop
    while(1);
}
