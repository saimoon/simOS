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

# Declare constants used for creating a multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard. We put this into a special
# section so we can force the header to be in the start of the final program.
# You don't need to understand all these details as it is just magic values that
# is documented in the multiboot standard. The bootloader will search for this
# magic sequence and recognize us as a multiboot kernel.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Currently the stack pointer register (esp) points at anything and using it may
# cause massive harm. Instead, we'll provide our own stack. We will allocate
# room for a small temporary stack by creating a symbol at the bottom of it,
# then allocating 16384 bytes for it, and finally creating a symbol at the top.
.section .bootstrap_stack
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The linker script specifies _start as the entry point to the kernel and the
# bootloader will jump to this position once the kernel has been loaded. It
# doesn't make sense to return from this function as the bootloader is gone.
.section .text
.global _start
_start:
    # Welcome to kernel mode! We now have sufficient code for the bootloader to
    # load and run our operating system.

    # By now, you are perhaps tired of assembly language. You realize some
    # things simply cannot be done in C, such as making the multiboot header in
    # the right section and setting up the stack. However, you would like to
    # write the operating system in a higher level language, such as C or C++.
    # To that end, the next task is preparing the processor for execution of
    # such code. C doesn't expect much at this point and we only need to set up
    # a stack. Note that the processor is not fully initialized yet and stuff
    # such as floating point instructions are not available yet.

    # To set up a stack, we simply set the esp register to point to the top of
    # our stack (as it grows downwards).
    movl $stack_top, %esp

    # Reset EFLAGS
    pushl $0
    popf

    # Push multiboot structure and magic value
    pushl %ebx
    pushl %eax

    # We are now ready to actually execute C code. We cannot embed that in an
    # assembly file, so we'll create a kernel.c file in a moment. In that file,
    # we'll create a C entry point called kernel_main and call it here.
    call kernel_main

    # In case the function returns, we'll want to put the computer into an
    # infinite loop. To do that, we use the clear interrupt ('cli') instruction
    # to disable interrupts, the halt instruction ('hlt') to stop the CPU until
    # the next interrupt arrives, and jumping to the halt instruction if it ever
    # continues execution, just to be safe.
    cli
hang:
    hlt
    jmp hang
