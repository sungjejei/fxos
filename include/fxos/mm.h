#ifndef FXOS_MM_H
#define FXOS_MM_H

#include <fxos/types.h>

extern uint8_t _KERNEL_DIRECT_BASE, _KERNEL_IMAGE_BASE;

static inline uintptr_t __forceinline KERNEL_DIRECT_MAP_BASE(void)
{
    uintptr_t addr;
    __asm__ volatile ("movabs %1, %0":"=r"(addr):"i"(&_KERNEL_DIRECT_BASE));
    return addr;
}

static inline uintptr_t __forceinline KERNEL_IMAGE_MAP_BASE(void)
{
    uintptr_t addr;
    __asm__ volatile ("movabs %1, %0":"=r"(addr):"i"(&_KERNEL_DIRECT_BASE));
    return addr;
}

#define PAGE_SIZE (0x1000)
#define PAGE_SHIFT (12)
#define PAGE_MASK (~(PAGE_SIZE - 1))

#define INVALID_ADDRESS (uintptr_t)(-1)

typedef uintptr_t phys_addr_t, virt_addr_t;
typedef uintptr_t page_index_t;
typedef size_t page_count_t;

#define FRAME_RESERVED 0x0001
#define FRAME_BAD 0x0002
#define MAX_ORDER 0

struct frame {
    unsigned short refcount;
    unsigned short flags;
    int order;
    struct frame *next;
    struct {
        void *freelist;
        struct frame *next;
        int order;
    } slab;
};

phys_addr_t get_frame_addr(struct frame *frame);
struct frame *get_frame_by_addr(phys_addr_t addr);
phys_addr_t get_frame_addr(struct frame *frame);
struct frame *get_frame_by_addr(phys_addr_t addr);
int init_frames(page_count_t count);
int init_buddy(page_index_t begin, page_count_t len);
struct frame *alloc_frame(void);
void free_frame(struct frame *frame);
void *kmalloc(size_t len);
void kfree(void *addr);

#endif
