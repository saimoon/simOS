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

// simOS includes
#include "int_vectors.h"

// Kernel data descriptor number (copy the value from "mem.h")
#define KERNEL_DS   $0x10


.text

// Initial ISR/IRQ handlers
.globl  isr_handler
.globl  irq_handler


// Stub for an ISR without its own error code 
.macro ISR_NOERRCODE, intno
.globl vector_isr\intno
vector_isr\intno:
    cli                                     // Disable interrupts firstly
    push    $0                              // Push a dummy error code
    push    $\intno                         // Push the interrupt number
    jmp     isr_common                      // Go to the common ISR handler code
.endm


// Stub for an ISR with its own error code 
.macro ISR_ERRCODE, intno
.globl vector_isr\intno
vector_isr\intno:
    cli                                     // Disable interrupts firstly
    push    $\intno                         // Push the interrupt number
    jmp     isr_common                      // Go to the common ISR handler code
.endm


// Stub for an IRQ
//   the first parameter is the IRQ number, 
//   the second is the ISR number it is remapped to 
.macro IRQ, irqno, intno
.globl vector_irq\irqno
vector_irq\irqno:
    cli                                     // Disable interrupts firstly
    push    $0                              // Push a dummy error code
    push    $\intno                         // Push the interrupt number
    jmp     irq_common                      // Go to the common IRQ handler code
.endm



/* IDT Vectors */

ISR_NOERRCODE       ISR0
ISR_NOERRCODE       ISR1
ISR_NOERRCODE       ISR2
ISR_NOERRCODE       ISR3
ISR_NOERRCODE       ISR4
ISR_NOERRCODE       ISR5
ISR_NOERRCODE       ISR6
ISR_NOERRCODE       ISR7
ISR_ERRCODE         ISR8
ISR_NOERRCODE       ISR9
ISR_ERRCODE         ISR10
ISR_ERRCODE         ISR11
ISR_ERRCODE         ISR12
ISR_ERRCODE         ISR13
ISR_ERRCODE         ISR14
ISR_NOERRCODE       ISR15
ISR_NOERRCODE       ISR16
ISR_NOERRCODE       ISR17
ISR_NOERRCODE       ISR18
ISR_NOERRCODE       ISR19
ISR_NOERRCODE       ISR20
ISR_NOERRCODE       ISR21
ISR_NOERRCODE       ISR22
ISR_NOERRCODE       ISR23
ISR_NOERRCODE       ISR24
ISR_NOERRCODE       ISR25
ISR_NOERRCODE       ISR26
ISR_NOERRCODE       ISR27
ISR_NOERRCODE       ISR28
ISR_NOERRCODE       ISR29
ISR_NOERRCODE       ISR30
ISR_NOERRCODE       ISR31
IRQ                 0,      IRQ0
IRQ                 1,      IRQ1
IRQ                 2,      IRQ2
IRQ                 3,      IRQ3
IRQ                 4,      IRQ4
IRQ                 5,      IRQ5
IRQ                 6,      IRQ6
IRQ                 7,      IRQ7
IRQ                 8,      IRQ8
IRQ                 9,      IRQ9
IRQ                 10,     IRQ10
IRQ                 11,     IRQ11
IRQ                 12,     IRQ12
IRQ                 13,     IRQ13
IRQ                 14,     IRQ14
IRQ                 15,     IRQ15


// Common ISR logic.
// It saves the processor state, sets up for kernel mode data segments, 
// calls the C-level fault handler, and finally restores the stack frame.
.type isr_common, @function
isr_common:
    // save registers of interrupted task
    pusha
    pushl    %ds

    // switch to kernel's data segment
    movl    KERNEL_DS, %eax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs

    // The current value of the ESP points to the beginning of the state save structure.
    // Save that on the stack as the input parameter to isr_handler.
    movl    %esp, %eax
    pushl   %eax
    call    isr_handler
    jmp     .Lreturn
.size isr_common, . - isr_common


// Common IRQ logic. 
// It saves the processor state, sets up for kernel mode data segments, 
// calls the C-level fault handler, and finally restores the stack frame.
.type irq_common, @function
irq_common:
    // save registers of interrupted task
    pusha
    pushl    %ds

    // switch to kernel's data segment
    movl    KERNEL_DS, %eax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs

    // The current value of the ESP points to the beginning of the state save structure.
    // Save that on the stack as the input parameter to isr_handler.
    movl    %esp, %eax
    pushl   %eax
    call    irq_handler

    // common return point for both isr_handler and irq_handler
.Lreturn:
    addl    $4, %esp

    // <TODO> Check for a context switch </TODO>

    // restore registers of interrupted task
    popl    %ds
    popa

    // reload the original data segment descriptor
    movw    %ds, %ax
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs

    addl    $8, %esp                        // Cleans up the pushed error code and pushed ISR number
    iret                                    // Pops 3-5 things at once: CS, EIP, EFLAGS (and maybe SS and ESP) */
.size irq_common, . - irq_common
.end
