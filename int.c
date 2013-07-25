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
#include "utlist.h"
#include "console.h"
#include "multiboot.h"
#include "mem.h"
#include "int_vectors.h"
#include "int.h"


/* ====== Globals ====== */

idt_t  kidt[IDT_ENTRIES+1];             // Interrupt Descriptor Table
idtr_t kidtr;                           // Interrupt Descriptor Table Register

void *int_handler_table[NR_IRQS];       // interrupt handler function table



/* ====== int handler functions ====== */

static uint32_t *common_handler(int irq, uint32_t *regs)
{
    // Deliver the IRQ
//  irq_dispatch(irq, regs);
    
    console__printf("Unexpected interrupt %d\n", irq);
    HALT();

    return regs; 
}


uint32_t *isr_handler(uint32_t *regs)
{
    uint32_t *ret;

    // Dispatch the interrupt
    ret = common_handler((int)regs[REG_IRQNO], regs);

    return ret;
}


uint32_t *irq_handler(uint32_t *regs)
{
    uint32_t *ret;
    int irq;

    // Get the IRQ number
    irq = (int)regs[REG_IRQNO];

    // Send an EOI (end of interrupt) signal to the PICs
    if (irq >= IRQ8) {
        // Send reset signal to slave
        outb(PIC_OCW2_EOI_NONSPEC, PIC2_OCW2);
    }

    // Send reset signal to master
    outb(PIC_OCW2_EOI_NONSPEC, PIC1_OCW2);

    // Dispatch the interrupt
    ret = common_handler(irq, regs);

    return ret;
}



/* ====== PRIVATE int functions ====== */

static void __set_idt(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
   kidt[num].lobase   = base & 0xFFFF;
   kidt[num].hibase   = (base >> 16) & 0xFFFF;
   kidt[num].sel      = sel;
   kidt[num].zero     = 0;
   kidt[num].flags    = flags;
}


// Load Interrupt Description Table
static inline void __load_idt(void)
{
  asm("lidtl %0" : : "m" (kidtr));
}



/* ====== PUBLIC int functions ====== */

void int__idt_init(void)
{
    memset(&kidt, 0, sizeof(idt_t)*IDT_ENTRIES);

    __set_idt(ISR0, (uint32_t)vector_isr0, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR1, (uint32_t)vector_isr1, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR2, (uint32_t)vector_isr2, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR3, (uint32_t)vector_isr3, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR4, (uint32_t)vector_isr4, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR5, (uint32_t)vector_isr5, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR6, (uint32_t)vector_isr6, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR7, (uint32_t)vector_isr7, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR8, (uint32_t)vector_isr8, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR9, (uint32_t)vector_isr9, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR10, (uint32_t)vector_isr10, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR11, (uint32_t)vector_isr11, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR12, (uint32_t)vector_isr12, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR13, (uint32_t)vector_isr13, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR14, (uint32_t)vector_isr14, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR15, (uint32_t)vector_isr15, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR16, (uint32_t)vector_isr16, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR17, (uint32_t)vector_isr17, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR18, (uint32_t)vector_isr18, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR19, (uint32_t)vector_isr19, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR20, (uint32_t)vector_isr20, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR21, (uint32_t)vector_isr21, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR22, (uint32_t)vector_isr22, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR23, (uint32_t)vector_isr23, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR24, (uint32_t)vector_isr24, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR25, (uint32_t)vector_isr25, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR26, (uint32_t)vector_isr26, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR27, (uint32_t)vector_isr27, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR28, (uint32_t)vector_isr28, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR29, (uint32_t)vector_isr29, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR30, (uint32_t)vector_isr30, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(ISR31, (uint32_t)vector_isr31, KERNEL_CS, DEF_INTGATE_FLAGS);

    __set_idt(IRQ0, (uint32_t)vector_irq0, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ1, (uint32_t)vector_irq1, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ2, (uint32_t)vector_irq2, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ3, (uint32_t)vector_irq3, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ4, (uint32_t)vector_irq4, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ5, (uint32_t)vector_irq5, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ6, (uint32_t)vector_irq6, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ7, (uint32_t)vector_irq7, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ8, (uint32_t)vector_irq8, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ9, (uint32_t)vector_irq9, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ10, (uint32_t)vector_irq10, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ11, (uint32_t)vector_irq11, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ12, (uint32_t)vector_irq12, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ13, (uint32_t)vector_irq13, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ14, (uint32_t)vector_irq14, KERNEL_CS, DEF_INTGATE_FLAGS);
    __set_idt(IRQ15, (uint32_t)vector_irq15, KERNEL_CS, DEF_INTGATE_FLAGS);

    kidtr.limit = sizeof(idt_t) * IDT_ENTRIES;
    kidtr.base  = (uint32_t)&kidt;

    __load_idt();
}
