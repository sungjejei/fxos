#include <fxos/mm.h>
#include <fxos/bootmm.h>
#include <fxos/kernel.h>
#include <multiboot2.h>

static int is_region_contains(const struct multiboot2_mmap_entry *e, uintptr_t begin, uintptr_t limit)
{
    return (begin >= e->addr) && (limit <= (e->addr + e->len));
}

int mm_init(struct multiboot2_mmap *mmap)
{
    const struct multiboot2_mmap_entry *entries = mmap->entries;
    size_t count = (mmap->head.size - sizeof(mmap->head)) / mmap->entry_size;

    /*
     * Initialize bootmm.
     */
    
    size_t maxi = 0;
    size_t maxlen = 0;

    for (size_t i = 0; i < count; i++) {
        if (entries[i].type == 1 && entries[i].len > maxlen) {
            maxi = i;
            maxlen = entries[i].len;
        }
    }

    if (maxlen == 0)
        panic("memory initialization failed: no usable RAM");

    kprintf("mm: Boot memory manager will be initialized for the region [0x%X-0x%X]\n",
            entries[maxi].addr, entries[maxi].addr + entries[maxi].len - 1);

    if (is_region_contains(&entries[maxi], KERNEL_IMAGE_PHYS_START(), KERNEL_IMAGE_PHYS_END()))
        bootmm_init(ALIGN_UP(KERNEL_IMAGE_PHYS_END(), PAGE_SIZE), entries[maxi].addr + entries[maxi].len);
    else
        bootmm_init(entries[maxi].addr, entries[maxi].addr + entries[maxi].len);
    
    /*
     * Initialize the frames.
     */
    
    phys_addr_t limit = 0;

    for (size_t i = 0; i < count; i++)
        if (entries[i].type == 1 && entries[i].addr + entries[i].len >= limit)
            limit = entries[i].addr + entries[i].len;

    kprintf("mm: Page frames will be initialize for the limit=0x%X\n", limit);

    init_frames(limit >> PAGE_SHIFT);

    /*
     * Initialize the allocator.
     */
    
    for (size_t i = 0; i < count; i++) {
        if (entries[i].addr < 0x100000 || entries[i].type != 1)
            continue;

        phys_addr_t begin, end;

        if (is_region_contains(&entries[i], bootmm_get_base(), bootmm_get_tail())) {
            begin = ALIGN_UP(bootmm_get_tail(), PAGE_SIZE);
            end = entries[i].addr + entries[i].len;
        }
        else if (is_region_contains(&entries[i], KERNEL_IMAGE_PHYS_START(), KERNEL_IMAGE_PHYS_END())) {
            begin = ALIGN_UP(KERNEL_IMAGE_PHYS_END(), PAGE_SIZE);
            end = entries[i].addr + entries[i].len;
        }
        else {
            begin = entries[i].addr;
            end = entries[i].addr + entries[i].len;
        }

        kprintf("mm: Page frame allocator will be initialized for the region [0x%X-0x%X]\n",
                begin, end - 1);

        init_buddy(begin >> PAGE_SHIFT, end >> PAGE_SHIFT);
    }

    kprintf("mm: Memory initialization succeed\n");

    return 0;
}
