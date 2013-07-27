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
#include "console.h"
#include "multiboot.h"
#include "mem.h"
#include "int_vectors.h"
#include "int.h"
#include "utlist.h"



/* ====== Globals ====== */

gdt_t  kgdt[GDT_NUMBERS];               // GDT
gdtr_t kgdtr;                           // GDTR

uint32_t *kpage_dir;                    // Page directory
uint32_t *kpage_tab;                    // Page Table

uint32_t    nb_frames;                  // Total number of frames
frame_t *   kframelist;                 // Frame list
free_area_t free_area[BUDDY_MAX_ORDER];



/* ====== IRQ handler functions ====== */

// Page Fault interrupt handler
void isr_pagefault(uint8_t irq, uint32_t *regs)
{
    uint32_t faulting_address;
    uint32_t error_code;
    uint8_t present;
    uint8_t rw;
    uint8_t us;
    uint8_t reserved;

    // the fault address is stored in the CR2 register
    __asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    error_code = (uint32_t)regs[REG_ERRCODE];

    // Decode information from the error code
    present  = (uint8_t)!(error_code & 0x1);
    rw       = (uint8_t)(error_code & 0x2);
    us       = (uint8_t)(error_code & 0x4);
    reserved = (uint8_t)(error_code & 0x8);

    console__printf("PAGE FAULT: 0x%x ", faulting_address);

    if (present) {
        console__write("PRESENT ");
    }

    if (rw) {
        console__write("READ-ONLY ");
    }

    if (us) {
        console__write("USER-MODE ");
    } 
    else {
        console__write("KERNEL-MODE ");
    }
    
    if (reserved) {
        console__write("RESERVED ");
    }

    console__write("\n");
    HALT();
}



/* ====== PRIVATE mem functions ====== */

// Fill a GDT descriptor with its data
static void __set_gdt(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    kgdt[num].base_low      = (base & 0xFFFF);
    kgdt[num].base_middle   = (base >> 16) & 0xFF;
    kgdt[num].base_high     = (base >> 24) & 0xFF;
    kgdt[num].limit_low     = (limit & 0xFFFF);
    kgdt[num].granularity   = ((limit >> 16) & 0x0F) | (gran & 0xF0);
    kgdt[num].access        = access;
}


// Load GDT on cpu using gdtl register
static inline void __load_gdt(void)
{
    asm("lgdtl %0           \n"
        "movw %1,   %%ax    \n"
        "movw %%ax, %%ds    \n"
        "movw %%ax, %%es    \n"
        "movw %%ax, %%fs    \n"
        "movw %%ax, %%gs    \n"
        "movw %%ax, %%ss    \n"
        "ljmp %2, $.flush   \n"
        ".flush:            "
        : : "m" (kgdtr), "i" (KERNEL_DS), "i" (KERNEL_CS) : "eax"
    );
}


// Extract memory layout info from multiboot struct filled at boot by GRUB
static void __get_multiboot_info(multiboot_info_t *mbi, memphy_layout_t *layout)
{
    extern uint32_t __TEXT_START, __BSS_END;
    layout->memsize_kb = 1024 + (uint32_t)mbi->mem_upper;
    layout->memsize_nframes = layout->memsize_kb / (PAGE_SIZE/1024);
    layout->phyaddr_kernel_start = ALIGN_PAGE((uint32_t) &__TEXT_START);
    layout->phyaddr_kernel_end = ALIGN_PAGE((uint32_t) &__BSS_END);
}


static void __init_freearea()
{
    uint32_t i;

    for (i=0; i<BUDDY_MAX_ORDER; i++) {
        free_area[i].free_list = NULL;
        free_area[i].map = 0;
    }
}


static void __init_framelist(uint32_t nframes, uint32_t kernel_end_addr)
{
    uint32_t i;

    nb_frames = nframes;                        // number of frame pages
    kframelist = (frame_t *) kernel_end_addr;   // frame list init
    for (i=0; i<nb_frames; i++) {
        kframelist[i].state = FRAME_UNDEF;
        kframelist[i].next = kframelist[i].prev = NULL;
    }
}


static uint32_t *__get_kframe(void)
{
    uint32_t i;

    for (i=0; i<nb_frames; i++) {
        if (kframelist[i].state == FRAME_KERNEL) {
            kframelist[i].state = FRAME_KUSED;
            return ((uint32_t *)(i * PAGE_SIZE));
        }
    }

    // out of memory
    console__printf("Fatal Error [Out of memory]: no Kernel Frame available !\n");
    hlt();
    return NULL;
}


static void __set_frame_state(uint32_t start, uint32_t len, frame_state_t state)
{
    uint32_t i;

/***
    console__printf("__set_frame_state: 0x%x - 0x%x (%d)\n", start, start+len, state);
***/

    if (FRAME(start+len-1) >= nb_frames) {
        console__printf("Error [Out of memory]: __set_frame_state() frame >= nb_frames\n");
        return;
    }

    if (state == FRAME_AVAIL) {
        for (i=start; i<(start+len); i+=PAGE_SIZE) {
            if (kframelist[FRAME(i)].state != FRAME_UNDEF) {
                console__printf("Error: __set_frame_state() frame %d (addr 0x%x) state already defined\n", FRAME(i), i);
                return;
            }
        }
    }

    for (i=start; i<(start+len); i+=PAGE_SIZE) {
        kframelist[FRAME(i)].state = state;
    }

    if (state == FRAME_AVAIL) {
        uint32_t j;
        uint32_t num_frames, current_frame, order, frames_per_order, num_order;
        num_frames = FRAME(len);            // number of frames in the memory region
        current_frame = FRAME(start);       // first frame in the memory region
        for (i=BUDDY_MAX_ORDER; i>0; i--) {
            order = i - 1;
            frames_per_order = 1 << order;
            num_order = num_frames >> order; 
            if (num_order > 0) {
                for (j=0; j<num_order; j++) {
                    DL_APPEND(free_area[order].free_list, &kframelist[current_frame+j*frames_per_order]);
                }
                current_frame += num_order*frames_per_order;
                num_frames -= num_order*frames_per_order;
            }
        }
    }
}


static void __dump_free_area()
{
    uint32_t i;
    uint32_t count;
    frame_t *elt;

    console__printf("Free Area list:\n");
    for (i=0; i<BUDDY_MAX_ORDER; i++) {
        DL_COUNT(free_area[i].free_list, elt, count);
        console__printf("Order[%d] num_frame=%d\n", i, count);
    }
}


static void __scan_memory_map(multiboot_info_t *mbi)
{
    if (CHECK_FLAG (mbi->flags, 6))
    {
        memory_map_t *mmap;

        for (mmap = (memory_map_t *) mbi->mmap_addr;
             (uint32_t) mmap < mbi->mmap_addr + mbi->mmap_length;
             mmap = (memory_map_t *) ((uint32_t) mmap + mmap->size + sizeof (mmap->size)))
        {
            if ((uint32_t)mmap->type == 1)
                __set_frame_state(mmap->base_addr_low, 
                                  mmap->length_low,
                                  FRAME_AVAIL);
            else
                __set_frame_state(mmap->base_addr_low, 
                                  mmap->length_low,
                                  FRAME_RESERV);
        }
    }
}



/* ====== PUBLIC mem functions ====== */

// Initialize BSS memory region
void mem__bss_init(void)
{
    extern uint32_t __BSS_START, __BSS_END;
    memset(&__BSS_START, '\0', (&__BSS_END - &__BSS_START));
}


// Initialize GDT  with a flat memory layout model
void mem__gdt_init(void)
{
    __set_gdt(0, 0x00000000, 0x00000000, 0x00, 0x00);  /* seg 0x00 */
    __set_gdt(1, 0x00000000, 0xFFFFFFFF, 0x9A, 0xCF);  /* seg 0x08 - kernel land CS (KERNEL_CS)*/
    __set_gdt(2, 0x00000000, 0xFFFFFFFF, 0x92, 0xCF);  /* seg 0x10 - kernel land DS ES FS GS SS (KERNEL_DS) */
    __set_gdt(3, 0x00000000, 0xFFFFFFFF, 0xFA, 0xCF);  /* seg 0x18 - User land CS */
    __set_gdt(4, 0x00000000, 0xFFFFFFFF, 0xF2, 0xCF);  /* seg 0x20 - User land DS ES FS GS SS */
//  __set_gdt(5, (uint32_t)&ktss, sizeof(struct tss), 0xe9, 0x00);  /* seg 0x38 - TSS */

    kgdtr.limit = GDT_NUMBERS * sizeof(gdt_t);
    kgdtr.base  = (uint32_t)&kgdt;

    __load_gdt();
}


// Initialize paging (MMU)
void mem__paging_init(uint32_t multiboot_info_addr)
{
    multiboot_info_t *mbi;
    memphy_layout_t kmemlayout;
    union addr_u addr;
    uint32_t i;

    mbi = (multiboot_info_t *) multiboot_info_addr;

    __get_multiboot_info(mbi, &kmemlayout);

/*
    console__printf("Physic Memory Layout:\n");
    console__printf("MemSize (Kb) = 0x%x\n", kmemlayout.memsize_kb);
    console__printf("MemSize (Num Frames) = 0x%x\n", kmemlayout.memsize_nframes);
    console__printf("Kernel Start (phyaddr) = 0x%x\n", kmemlayout.phyaddr_kernel_start);
    console__printf("Kernel End (phyaddr) = 0x%x\n", kmemlayout.phyaddr_kernel_end);

    console__printf("kgdtr addr = 0x%p\n", &kgdtr);
*/

    __init_framelist(kmemlayout.memsize_nframes, kmemlayout.phyaddr_kernel_end);

    __init_freearea();


    __scan_memory_map(mbi);

    __set_frame_state(0x00000000, 0x00020000, FRAME_RESERV);
    __set_frame_state(kmemlayout.phyaddr_kernel_start, KERNEL_RESERVED_MEM, FRAME_KERNEL);
    __set_frame_state(kmemlayout.phyaddr_kernel_start, kmemlayout.phyaddr_kernel_end-kmemlayout.phyaddr_kernel_start, FRAME_KUSED);
    __set_frame_state((uint32_t)kframelist, (uint32_t)nb_frames*sizeof(frame_t), FRAME_KUSED);  

/*
    __dump_free_area();
*/

    // * paging: map one-to-one first 4Mb of ram *

    // get 1 frame each for one page dir and one page table (1024*4b)
    kpage_dir = __get_kframe();
    kpage_tab = __get_kframe();
  
    // init page directory (only 1 descriptor)
    addr.addr = (uint32_t)kpage_tab;
    addr.page_dir.r = 1;              // read-write
    addr.page_dir.p = 1;              // present
    kpage_dir[0] = addr.addr;
    for (i=1; i<1024; i++)
        kpage_dir[i] = 0;

    // Init page table (0x00000000 - 0x00400000)
    for (i=0; i<1024; i++) {
        addr.addr = (i * PAGE_SIZE);
        addr.page_tab.r = 1;
        addr.page_tab.p = 1;
        kpage_tab[i] = addr.addr;
    }

    EnablePaging();

/***
    console__printf("kpage_dir = 0x%x\n", kpage_dir);
    console__printf("kpage_tab = 0x%x\n", kpage_tab);
    console__printf("kframelist = 0x%x\n", kframelist);
***/
}


// attach ISR14 to page fault irq handler
void mem__pagefaultirq(void)
{
    // Attach IRQ0 to the timer interrupt handler
    int__irq_attach(ISR14, (irqvfunc_t)isr_pagefault);

    // Enable IRQ0
    int__enable_irq(ISR14);
}


void mem__dump_map(void)
{
    extern uint32_t __TEXT_START, __TEXT_END;
    extern uint32_t __RODATA_START, __RODATA_END;
    extern uint32_t __DATA_START, __DATA_END;
    extern uint32_t __BSS_START, __BSS_END;

    console__printf("Sections Memory addresses:\n");
    console__printf("TEXT Start   = %p\n", (void*)&__TEXT_START);
    console__printf("TEXT End     = %p\n", (void*)&__TEXT_END);
    console__printf("-------------------------\n");
    console__printf("RODATA Start = %p\n", (void*)&__RODATA_START);
    console__printf("RODATA End   = %p\n", (void*)&__RODATA_END);
    console__printf("-------------------------\n");
    console__printf("DATA Start   = %p\n", (void*)&__DATA_START);
    console__printf("DATA End     = %p\n", (void*)&__DATA_END);
    console__printf("-------------------------\n");
    console__printf("BSS Start    = %p\n", (void*)&__BSS_START);
    console__printf("BSS End      = %p\n", (void*)&__BSS_END);
    console__printf("-------------------------\n\n");
}
