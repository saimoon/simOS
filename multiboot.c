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

/* http://www.gnu.org/software/grub/manual/multiboot/multiboot.html */

#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"
#include "console.h"


/* ====== PUBLIC multiboot functions ====== */

void multiboot__dump_info(uint32_t magic, uint32_t addr)
{
    multiboot_info_t *mbi;


    console__write("Dump Multiboot info:\n");

    /* Check Multiboot-compliant boot loader */
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        console__printf("Invalid magic number: 0x%x\n", (uint32_t)magic);
        return;
    }

    /* Set MBI to the address of the Multiboot information structure. */
    mbi = (multiboot_info_t *) addr;

    /* Print out the flags. */
    console__printf("flags = 0x%x\n", (uint32_t)mbi->flags);

    /* Are mem_* valid? */
    if (CHECK_FLAG(mbi->flags, 0))
        console__printf("mem_lower = %uKB, mem_upper = %uKB\n", 
                (uint32_t)mbi->mem_lower, (uint32_t)mbi->mem_upper);
     
    /* Is boot_device valid? */
    if (CHECK_FLAG(mbi->flags, 1))
        console__printf("boot_device = 0x%x\n", (uint32_t)mbi->boot_device);
     
    /* Is the command line passed? */
    if (CHECK_FLAG(mbi->flags, 2))
        console__printf("cmdline = %s\n", (char *)mbi->cmdline);

    /* Are mods_* valid? */
    if (CHECK_FLAG(mbi->flags, 3))
    {
        module_t *mod;
        unsigned int i;
     
        console__printf("mods_count = %d, mods_addr = 0x%x\n",
                       (int) mbi->mods_count, (int) mbi->mods_addr);
        for (i = 0, mod = (module_t *) mbi->mods_addr;
             i < mbi->mods_count;
             i++, mod++)
            console__printf(" mod_start = 0x%x, mod_end = 0x%x, cmdline = %s\n",
                            (uint32_t) mod->mod_start,
                            (uint32_t) mod->mod_end,
                            (char *) mod->string);
    }
     
    /* Bits 4 and 5 are mutually exclusive! */
    if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
    {
        console__printf("Both bits 4 and 5 are set.\n");
        return;
    }
     
    /* Is the symbol table of a.out valid? */
    if (CHECK_FLAG (mbi->flags, 4))
    {
        aout_symbol_table_t *aout_sym = &(mbi->u.aout_sym);
     
        console__printf("aout_symbol_table: tabsize = 0x%0x, "
                       "strsize = 0x%x, addr = 0x%x\n",
                       (uint32_t) aout_sym->tabsize,
                       (uint32_t) aout_sym->strsize,
                       (uint32_t) aout_sym->addr);
    }
     
    /* Is the section header table of ELF valid? */
    if (CHECK_FLAG (mbi->flags, 5))
    {
        elf_section_header_table_t *elf_sec = &(mbi->u.elf_sec);
     
        console__printf("elf_sec: num = %u, size = 0x%x,"
                       " addr = 0x%x, shndx = 0x%x\n",
                       (uint32_t) elf_sec->num, (uint32_t) elf_sec->size,
                       (uint32_t) elf_sec->addr, (uint32_t) elf_sec->shndx);
    }
     
    /* Are mmap_* valid? */
    if (CHECK_FLAG (mbi->flags, 6))
    {
        memory_map_t *mmap;

        console__printf("MULTIBOOT MEMORY MAP\n");    
        console__printf("mmap_addr = 0x%x, mmap_length = 0x%x\n",
                       (uint32_t) mbi->mmap_addr, (uint32_t) mbi->mmap_length);
        for (mmap = (memory_map_t *) mbi->mmap_addr;
             (uint32_t) mmap < mbi->mmap_addr + mbi->mmap_length;
             mmap = (memory_map_t *) ((uint32_t) mmap
                    + mmap->size + sizeof (mmap->size)))
        {
            console__printf(" size = 0x%x, base_addr = 0x%x%x,"
                           " length = 0x%x%x, type = 0x%x\n",
                           (uint32_t) mmap->size,
                           mmap->base_addr_high,
                           mmap->base_addr_low,
                           mmap->length_high,
                           mmap->length_low,
                           (uint32_t) mmap->type);
        }
    }
}
