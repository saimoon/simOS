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


// Programmable Interrupt Controller (PIC)
#define PIC_MASTER_CMD          0x20
#define PIC_MASTER_DATA         0x21
#define PIC_SLAVE_CMD           0xA0
#define PIC_SLAVE_DATA          0xA1
#define PIC_MASTER              PIC_MASTER_DATA
#define PIC_SLAVE               PIC_SLAVE_DATA
#define PIC_ICW1_IC4            0x01
#define PIC_ICW1_INIT           0x10
#define PIC_ICW4_8086           0x01
#define PIC_ICW4_AEOI           0x02
#define PIC_EOI                 0x20
#define PIC_ICW3_M_CASCADE  	0x04
#define PIC_ICW3_S_CASCADE  	0x02
#define IRQ0_VECTOR             0x20
#define IRQ8_VECTOR             0x28
#define PIC_MASTER_MASK         0xFB 	// We can't disable IRQ2 because slave it's there
#define PIC_SLAVE_MASK          0xFF


// FLAGS bits
#define X86_FLAGS_CF            (1 << 0)  /* Bit 0:  Carry Flag */
                                          /* Bit 1:  Reserved */
#define X86_FLAGS_PF            (1 << 2)  /* Bit 2:  Parity Flag */
                                          /* Bit 3:  Reserved */
#define X86_FLAGS_AF            (1 << 4)  /* Bit 4:  Auxillary carry Flag */
                                          /* Bit 5:  Reserved */
#define X86_FLAGS_ZF            (1 << 6)  /* Bit 6:  Zero Flag */
#define X86_FLAGS_SF            (1 << 7)  /* Bit 7:  Sign Flag */
#define X86_FLAGS_TF            (1 << 8)  /* Bit 8:  Trap Flag */
#define X86_FLAGS_IF            (1 << 9)  /* Bit 9:  Interrupt Flag */
#define X86_FLAGS_DF            (1 << 10) /* Bit 10: Direction Flag */
#define X86_FLAGS_OF            (1 << 11) /* Bit 11: Overflow Flag */
#define X86_FLAGS_IOPL_SHIFT    (12)      /* Bits 12-13: IOPL mask (286+ only)*/
#define X86_FLAGS_IOPL_MASK     (3 << X86_FLAGS_IOPL_SHIFT)
#define X86_FLAGS_NT            (1 << 14) /* Bit 14: Nested Task */
                                          /* Bit 15: Reserved */



#define IDT_ENTRIES 0xFF			// number of IDT entries
#define DEF_INTGATE_FLAGS 0x8E		//P=1 DPL=0 (interrupt gate descriptor)


// Interrupt Descriptor Table
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

// Interrupt Descriptor Table Register
typedef
struct idtr
{
   uint16_t limit;
   uint32_t base;
} __attribute__((packed))
idtr_t;


// IRQ vector function type
typedef void (*irqvfunc_t)(uint8_t irq, uint32_t *context);


//  Interrupt Vectors
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
void int__irq_attach(uint8_t irq, irqvfunc_t isr);
void int__enable_irq(uint8_t irq);
void int__disable_irq(uint8_t irq);
uint32_t int__irqflags();
bool int__irqdisabled(uint32_t flags);
bool int__irqenabled(uint32_t flags);
void int__irqdisable(void);
void int__irqenable(void);
uint32_t int__irqsave(void);
void int__irqrestore(uint32_t flags);


#endif /* SIMOS_INT_H */
