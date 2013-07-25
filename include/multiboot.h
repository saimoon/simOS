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

#ifndef SIMOS_MULTIBOOT_H
#define SIMOS_MULTIBOOT_H


/* magic number for multiboot header */
#define MULTIBOOT_HEADER_MAGIC      0x1BADB002

/* flags for multiboot header */
#define MULTIBOOT_HEADER_FLAGS      0x00010003

/* magic number passed by multiboot-compliant boot loader.  */
#define MULTIBOOT_BOOTLOADER_MAGIC  0x2BADB002

/* The size of our stack (8KB).  */
#define STACK_SIZE          0x2000

/* C symbol format. HAVE_ASM_USCORE is defined by configure.  */
#ifdef HAVE_ASM_USCORE
# define EXT_C(sym)         _ ## sym
#else
# define EXT_C(sym)         sym
#endif

/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

#ifndef ASM
/* We don't want these declarations in boot.S  */

/* multiboot header */
typedef struct multiboot_header {
    unsigned long magic;
    unsigned long flags;
    unsigned long checksum;
    unsigned long header_addr;
    unsigned long load_addr;
    unsigned long load_end_addr;
    unsigned long bss_end_addr;
    unsigned long entry_addr;
} multiboot_header_t;

/* symbol table for a.out */
typedef struct aout_symbol_table {
    unsigned long tabsize;
    unsigned long strsize;
    unsigned long addr;
    unsigned long reserved;
} aout_symbol_table_t;

/* section header table for ELF */
typedef struct elf_section_header_table {
    unsigned long num;
    unsigned long size;
    unsigned long addr;
    unsigned long shndx;
} elf_section_header_table_t;

/* multiboot information */
typedef struct multiboot_info {
    unsigned long flags;
    unsigned long mem_lower;
    unsigned long mem_upper;
    unsigned long boot_device;
    unsigned long cmdline;
    unsigned long mods_count;
    unsigned long mods_addr;
    union {
        aout_symbol_table_t aout_sym;
        elf_section_header_table_t elf_sec;
    } u;
    unsigned long mmap_length;
    unsigned long mmap_addr;
} multiboot_info_t;

/* module structure */
typedef struct module {
    unsigned long mod_start;
    unsigned long mod_end;
    unsigned long string;
    unsigned long reserved;
} module_t;

/* memory map. Be careful that the offset 0 is base_addr_low
   but no size.  */
typedef struct memory_map {
    unsigned long size;
    unsigned long base_addr_low;
    unsigned long base_addr_high;
    unsigned long length_low;
    unsigned long length_high;
    unsigned long type;
} memory_map_t;

#endif              /* ! ASM */


/* PUBLIC multiboot functions */
void multiboot__dump_info(uint32_t magic, uint32_t addr);


#endif /* SIMOS_MULTIBOOT_H */
