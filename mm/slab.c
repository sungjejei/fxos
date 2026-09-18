#include <fxos/kernel.h>
#include <fxos/mm.h>
#include <fxos/rtl.h>

#define SLAB_MAX_ORDER 7

struct frame *slab[SLAB_MAX_ORDER + 1];

static int calc_order(size_t len)
{
    if (len <= 8) 
        return 0;
    else
        return msb_index(roundup_power_of_two(len)) - 4;
}

static size_t order_len(int order)
{
    return 16 * (1 << order);
}

static struct frame *create_slab(int order)
{
    struct frame *frame = alloc_frame();
    void *addr;

    if (!frame)
        return NULL;

    addr = (void*)(get_frame_addr(frame) + KERNEL_DIRECT_MAP_BASE());
    memset(addr, 0, PAGE_SIZE);

    frame->slab.next = NULL;
    frame->slab.freelist = addr;
    frame->slab.order = order;

    for (size_t i = 0; i < PAGE_SIZE - order_len(order); i += order_len(order)) {
        void **cur = (void**)((char*)addr + i);
        void *next = (void*)((char*)addr + i + order_len(order));
        *cur = next;
    }

    return frame;
}

void *kmalloc(size_t len)
{
    int order = calc_order(len);
    void *addr;
    
    while (slab[order] && !slab[order]->slab.freelist)
        slab[order] = slab[order]->slab.next;

    if (!slab[order]) {
        struct frame *frame = create_slab(order);
        if (!frame) return NULL;
        slab[order] = frame;
    }

    addr = slab[order]->slab.freelist;
    void **next = (void**)slab[order]->slab.freelist;

    slab[order]->slab.freelist = *next;
    
    return addr;
}

void kfree(void *addr)
{
    struct frame *frame = get_frame_by_addr((virt_addr_t)addr - KERNEL_DIRECT_MAP_BASE());

    if (!frame)
        panic("kfree failed");

    if (frame->slab.freelist == NULL) {
        frame->slab.next = slab[frame->slab.order];
        slab[frame->slab.order] = frame;
    }

    *(void**)addr = frame->slab.freelist;
    frame->slab.freelist = addr;
}
