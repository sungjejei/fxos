#include <fxos/mm.h>
#include <fxos/bootmm.h>
#include <fxos/kernel.h>

static phys_addr_t begin;
static phys_addr_t tail;
static phys_addr_t limit;

phys_addr_t bootmm_get_base(void)
{
    return begin;
}

phys_addr_t bootmm_get_tail(void)
{
    return tail;
}

phys_addr_t bootmm_get_limit(void)
{
    return limit;
}

int bootmm_init(phys_addr_t base, size_t len)
{
    begin = base;
    tail = base;
    limit = base + len;
    return 0;
}

void *bootmm_alloc(size_t len, size_t align)
{
    phys_addr_t addr = ALIGN_UP(tail, align);

    if (addr + len >= limit)
        panic("Boot memory allocation failed!");

    tail = addr + len;
    return (void*)(addr + KERNEL_DIRECT_MAP_BASE());
}
