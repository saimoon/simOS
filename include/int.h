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

#ifndef SIMOS_INT_H
#define SIMOS_INT_H


// Common register save structure created by ISR/IRQ interrupt processing
#define REG_DS            (0)  /* Data segment selector */
#define REG_EDI           (1)  /* Saved by pusha */
#define REG_ESI           (2)  /* "   " "" "   " */
#define REG_EBP           (3)  /* "   " "" "   " */
#define REG_ESP           (4)  /* "   " "" "   " (NOTE 1)*/
#define REG_EBX           (5)  /* "   " "" "   " */
#define REG_EDX           (6)  /* "   " "" "   " */
#define REG_ECX           (7)  /* "   " "" "   " */
#define REG_EAX           (8)  /* "   " "" "   " */
#define REG_IRQNO         (9)  /* Interrupt number (NOTE 2) */
#define REG_ERRCODE      (10)  /* Error code (NOTE 2) */
#define REG_EIP          (11)  /* Pushed by process on interrupt processing */
#define REG_CS           (12)  /* "    " "" "     " "" "       " "        " */
#define REG_EFLAGS       (13)  /* "    " "" "     " "" "       " "        " */
#define REG_SP           (14)  /* "    " "" "     " "" "       " "        " */
#define REG_SS           (15)  /* "    " "" "     " "" "       " "        " */


/* Programmable Interrupt Controller (PIC) */

// Operational Control Words
// The first instruction the Operation Control Word 1 (OCW1) to set which
// IRQ's to mask and which IRQ's not to.

#define PIC1_OCW1           0x20
#define PIC2_OCW1           0xa0

#define PIC1_OCW1_IRQ0		(1 << 0) /* IRQ0  System Timer */
#define PIC1_OCW1_IRQ1      (1 << 1) /* IRQ1  Keyboard */
#define PIC1_OCW1_IRQ2      (1 << 2) /* IRQ2  PIC2 */
#define PIC1_OCW1_IRQ3      (1 << 3) /* IRQ3  Serial Port */
#define PIC1_OCW1_IRQ4      (1 << 4) /* IRQ4  Serial Port */
#define PIC1_OCW1_IRQ5      (1 << 5) /* IRQ5  Reserved/Sound Card */
#define PIC1_OCW1_IRQ6      (1 << 6) /* IRQ6  Floppy Disk Controller */
#define PIC1_OCW1_IRQ7      (1 << 7) /* IRQ7  Parallel Port */
#define PIC1_OCW1_ALL

#define PIC2_OCW1_IRQ8      (1 << 0) /* IRQ8  Real Time Clock */
#define PIC2_OCW1_IRQ9      (1 << 1) /* IRQ9  Redirected IRQ2 */
#define PIC2_OCW1_IRQ10     (1 << 2) /* IRQ10 Reserved */
#define PIC2_OCW1_IRQ11     (1 << 3) /* IRQ11 Reserved */
#define PIC2_OCW1_IRQ12     (1 << 4) /* IRQ12 PS/2 Mouse */
#define PIC2_OCW1_IRQ13     (1 << 5) /* IRQ13 Maths Co-Processor */
#define PIC2_OCW1_IRQ14     (1 << 6) /* IRQ14 Hard Disk Drive */
#define PIC2_OCW1_IRQ15     (1 << 7) /* IRQ15 Reserved */
#define PIC2_OCW1_ALL

// Operation Control Word 2 selects how the End of Interrupt (EOI) procedure
// works. The only thing of interest to us in this register is the non-
// specific EOI command, which we must send at the end of our ISR's.

#define PIC1_OCW2          	0x20
#define PIC2_OCW2           0xa0

#define PIC_OCW2_ACT_SHIFT   (0)
#define PIC_OCW2_ACT_MASK    (7 << PIC_OCW2_ACT_SHIFT)
#define PIC1_OCW2_ACT_IRQ0   (0 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 0 */
#define PIC1_OCW2_ACT_IRQ1   (1 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 1 */
#define PIC1_OCW2_ACT_IRQ2   (2 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 2 */
#define PIC1_OCW2_ACT_IRQ3   (3 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 3 */
#define PIC1_OCW2_ACT_IRQ4   (4 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 4 */
#define PIC1_OCW2_ACT_IRQ5   (5 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 5 */
#define PIC1_OCW2_ACT_IRQ6   (6 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 6 */
#define PIC1_OCW2_ACT_IRQ7   (7 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 7 */

#define PIC2_OCW2_ACT_IRQ8   (0 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 8 */
#define PIC2_OCW2_ACT_IRQ9   (1 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 9 */
#define PIC2_OCW2_ACT_IRQ10  (2 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 10 */
#define PIC2_OCW2_ACT_IRQ11  (3 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 11 */
#define PIC2_OCW2_ACT_IRQ12  (4 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 12 */
#define PIC2_OCW2_ACT_IRQ13  (5 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 13 */
#define PIC2_OCW2_ACT_IRQ14  (6 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 14 */
#define PIC2_OCW2_ACT_IRQ15  (7 << PIC_OCW2_ACT_SHIFT) /* Act on IRQ 15 */

#define PIC_OCW2_EOI_SHIFT   (5)
#define PIC_OCW2_EOI_MASK    (7 << PIC_OCW2_EOI_SHIFT)
#define PIC_OCW2_EOI_AUTO    (0 << PIC_OCW2_EOI_SHIFT) /* Rotate in Auto EOI Mode (Clear) */
#define PIC_OCW2_EOI_NONSPEC (1 << PIC_OCW2_EOI_SHIFT) /* Non Specific EOI */
#define PIC_OCW2_EOI_SPEC    (3 << PIC_OCW2_EOI_SHIFT) /* Specific EOI */
#define PIC_OCW2_EOI_RAUTO   (4 << PIC_OCW2_EOI_SHIFT) /* Rotate in Auto EOI Mode (Set) */
#define PIC_OCW2_EOI_RNSPEC  (5 << PIC_OCW2_EOI_SHIFT) /* Rotate on Non-Specific EOI */
#define PIC_OCW2_EOI_PRIO    (6 << PIC_OCW2_EOI_SHIFT) /* Set Priority Command (Use Bits 2:0) */
#define PIC_OCW2_EOI_RSPEC   (7 << PIC_OCW2_EOI_SHIFT) /* Rotate on Specific EOI (Use Bits 2:0) */



#define IDT_ENTRIES 0xFF			// number of IDT entries

#define DEF_INTGATE_FLAGS 0x8E		//P=1 DPL=0 (interrupt gate descriptor)

typedef
struct idt
{
   uint16_t lobase;
   uint16_t sel;
   uint8_t  zero;
   uint8_t  flags;
   uint16_t hibase;
} __attribute__((packed))
idt_t;

typedef
struct idtr
{
   uint16_t limit;
   uint32_t base;
} __attribute__((packed))
idtr_t;


extern void vector_isr0(void);
extern void vector_isr1(void);
extern void vector_isr2(void);
extern void vector_isr3(void);
extern void vector_isr4(void);
extern void vector_isr5(void);
extern void vector_isr6(void);
extern void vector_isr7(void);
extern void vector_isr8(void);
extern void vector_isr9(void);
extern void vector_isr10(void);
extern void vector_isr11(void);
extern void vector_isr12(void);
extern void vector_isr13(void);
extern void vector_isr14(void);
extern void vector_isr15(void);
extern void vector_isr16(void);
extern void vector_isr17(void);
extern void vector_isr18(void);
extern void vector_isr19(void);
extern void vector_isr20(void);
extern void vector_isr21(void);
extern void vector_isr22(void);
extern void vector_isr23(void);
extern void vector_isr24(void);
extern void vector_isr25(void);
extern void vector_isr26(void);
extern void vector_isr27(void);
extern void vector_isr28(void);
extern void vector_isr29(void);
extern void vector_isr30(void);
extern void vector_isr31(void);
extern void vector_irq0(void);
extern void vector_irq1(void);
extern void vector_irq2(void);
extern void vector_irq3(void);
extern void vector_irq4(void);
extern void vector_irq5(void);
extern void vector_irq6(void);
extern void vector_irq7(void);
extern void vector_irq8(void);
extern void vector_irq9(void);
extern void vector_irq10(void);
extern void vector_irq11(void);
extern void vector_irq12(void);
extern void vector_irq13(void);
extern void vector_irq14(void);
extern void vector_irq15(void);


/* PUBLIC int functions */
void int__idt_init(void);


#endif /* SIMOS_INT_H */
