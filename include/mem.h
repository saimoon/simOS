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

#ifndef SIMOS_MEM_H
#define SIMOS_MEM_H


#define PAGE_SHIFT  12
#define PAGE_SIZE   (1 << PAGE_SHIFT)     // 2^12 = 0x1000 (4Kb)
#define PAGE_MASK   (~(PAGE_SIZE - 1))    // 0xFFFFF000

#define ALIGN(x,a) (((x)+(a)-1)&~((a)-1))
#define ALIGN_PAGE(x) (((x)+(PAGE_SIZE)-1)&~((PAGE_SIZE)-1))

#define PAGING_FLAG  0x80000000           // CR0 - bit 31
#define PAGE(addr)   ((addr) >> 12)
#define DIRE(addr)   ((addr) >> 22)
#define GET_PD(addr) ((addr) & 0xFFC00000) >> 22  // Return Page Directory
#define GET_PT(addr) ((addr) & 0x003FF000) >> 12  // Return Page Table

union addr_u
{
  struct
    {
      unsigned offset:12;
      unsigned ptab:10;
      unsigned pdir:10;
    } vaddr;

  struct
    {
      unsigned p:1;         /* Present          */
      unsigned r:1;         /* Read/write       */
      unsigned u:1;         /* User/Supervisor  */
      unsigned w:1;         /* Write Through    */
      unsigned d:1;         /* Cache Disabled   */
      unsigned a:1;         /* Accessed         */
      unsigned _unused:1;
      unsigned s:1;         /* Page Size        */
      unsigned g:1;         /* Global           */
      unsigned avail:3;
      unsigned pdir:20;
    } page_dir;

  struct
    {
      unsigned p:1;         /* Present          */
      unsigned r:1;         /* Read/write       */
      unsigned u:1;         /* User/Supervisor  */
      unsigned w:1;         /* Write Through    */
      unsigned c:1;         /* Cache Disabled   */
      unsigned a:1;         /* Accessed         */
      unsigned d:1;         /* Dirty            */
      unsigned _unused:1;
      unsigned g:1;         /* Global           */
      unsigned avail:3;
      unsigned ptab:20;
    } page_tab;

  uint32_t addr;
};

# define USER_VADDR             0x08040000
# define USER_VADDR_STACK       0xbfffffff
# define USER_VADDR_STACK_P     0xbffff000
# define KERNEL_VADDR           0xc0000000
# define KERNEL_VADDR_STACK     0xffffffff

// Enable paging
# define EnablePaging()                     \
  __asm__(                                  \
          /* PD_PHY_ADDR */                 \
          "mov  %0, %%eax\n"                \
          "mov  %%eax, %%cr3\n"             \
          "mov  %%cr0, %%eax\n"             \
          /* PAGING_FLAG */                 \
          "or   $0x80000000, %%eax\n"       \
          "mov  %%eax, %%cr0"               \
          ::"m"(kpage_dir)                  \
);


#define KERNEL_RESERVED_MEM 0x00100000
#define BUDDY_MAX_ORDER 10

#define FRAME(addr)   ((addr) >> 12)

// Frame page state
typedef enum {
  FRAME_UNDEF,
  FRAME_AVAIL,
  FRAME_RESERV,
  FRAME_KERNEL,
  FRAME_KUSED,
  FRAME_USED
} frame_state_t;
  
// Frame page struct
typedef struct frame {
  frame_state_t state;
  struct frame *next, *prev;
} frame_t;

typedef struct free_area_struct {
  frame_t *free_list;
  uint32_t map;
} free_area_t;


typedef
struct memphy_layout
{
    uint32_t memsize_kb;
    uint32_t memsize_nframes;
    uint32_t phyaddr_kernel_start;
    uint32_t phyaddr_kernel_end;
} __attribute__((packed))
memphy_layout_t;


#define GDT_NUMBERS  0x06

#define KERNEL_CS   0x08
#define KERNEL_DS   0x10
#define USER_CS     0x18
#define USER_DS     0x20

typedef 
struct gdt
{
  uint16_t  limit_low;
  uint16_t  base_low;
  uint8_t   base_middle;
  uint8_t   access;
  uint8_t   granularity;
  uint8_t   base_high;
} __attribute__((packed)) 
gdt_t;

typedef 
struct gdtr
{
   uint16_t limit;
   uint32_t base;
} __attribute__((packed))
gdtr_t;



/* PUBLIC mem functions */
void mem__bss_init(void);
void mem__gdt_init(void);
void mem__paging_init(uint32_t multiboot_info_addr);
void mem__dump_map(void);


#endif /* SIMOS_MEM_H */
