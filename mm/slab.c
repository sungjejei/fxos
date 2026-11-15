#include <fxos/types.h>
#include <fxos/mm.h>
#include <fxos/rtl.h>
#include <fxos/kernel.h>

#define SLAB_SIZE_COUNT 7

static struct frame *slabs[SLAB_SIZE_COUNT] = { NULL };

static int calc_slab_size_index(size_t len)
{
    return msb_index(len >> 6);
}

static struct frame *create_slab(size_t size)
{
    struct frame *frame = alloc_frame();

    if (!frame)
        return NULL;

    memset(&frame->slab, 0, sizeof(frame->slab));
    frame->flags |= FRAME_SLAB;
    frame->slab.len = size;

    return frame;
}

static int get_free_index(uint64_t bitmap)
{
    return __builtin_ctzll(~bitmap);
}

static void set_bit_index(uint64_t *bitmap, unsigned int index)
{
    asm volatile (
        "btsq %1, %0\n\t"
        :"+m"(*bitmap)
        :"r"((unsigned long)index)
        :"cc","memory"
    );
}

static int unset_bit_index(uint64_t *bitmap, unsigned int index)
{
    unsigned char old = 0;

    asm volatile (
        "btrq %2, %1\n\t"
        "setc %0\n\t"
        :"=q"(old),"+m"(*bitmap)
        :"r"((unsigned long)index)
        :"cc","memory"
    );

    return old;
}

/*
 * Allocates an object and returns its index.
 * On failure, returns -1.
 */
static int alloc_object(struct frame *slab)
{
    unsigned int index = get_free_index(slab->slab.bitmap);

    if (index >= PAGE_SIZE / slab->slab.len)
        return -1;

    set_bit_index(&slab->slab.bitmap, index);
    slab->slab.count++;

    return index;
}

/*
 * Frees an object.
 * Returns 0 on success, returns -1 on failure which means double-free.
 */
static int free_object(struct frame *slab, int index)
{
    int old = unset_bit_index(&slab->slab.bitmap, index);

    if (old == 0)
        return -1;

    slab->slab.count--;
    return 0;
}

static size_t compute_slab_len(size_t len)
{
    return len <= 64 ? 64 : roundup_power_of_two(len);
}

void *kmalloc(size_t len)
{
    if (len > PAGE_SIZE)
        return NULL;

    len = compute_slab_len(len);
    int index = calc_slab_size_index(len);

    struct frame *slab = slabs[index];

    /* Look for an empty slab. */
    while (slab && slab->slab.count >= PAGE_SIZE / slab->slab.len)
        slab = slab->slab.next;

    /* If every slabs are full, create a new slab. */
    if (!slab) {
        struct frame *new_slab = create_slab(len);

        if (!new_slab)
            return NULL;

        new_slab->slab.next = slabs[index];
        slabs[index] = new_slab;
        slab = new_slab;
    }

    /* Allocate an object in the slab. */
    int object_index = alloc_object(slab);

    void *addr = (void*)(get_frame_addr(slab) + KERNEL_DIRECT_MAP_BASE());
    return (void*)((char*)addr + (object_index * slab->slab.len));
}

void kfree(void *addr)
{
    if (!addr)
        return;

    struct frame *slab = get_frame_by_addr((virt_addr_t)addr - KERNEL_DIRECT_MAP_BASE());

    if (!slab || !(slab->flags & FRAME_SLAB))
        panic("kfree fatal: invalid address");

    int index = ((uintptr_t)addr - ALIGN_DOWN(addr, PAGE_SIZE)) / slab->slab.len;

    if (free_object(slab, index) != 0)
        panic("kfree fatal: double free");

    /* If the slab is empty, free the slab page frame. */
    if (slab->slab.count == 0) {
        kprintf("kfree: slab is empty, freeing frame.\n");

        int slabs_index = calc_slab_size_index(slab->slab.len);
        struct frame *cur = slabs[slabs_index];
        struct frame *prev = NULL;

        while (cur != slab) {
            prev = cur;
            cur = cur->slab.next;
        }

        if (!prev)
            slabs[slabs_index] = cur->slab.next;
        else
            prev->slab.next = cur->slab.next;

        free_frame(slab);
    }
}
