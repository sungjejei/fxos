#ifndef FXOS_BOOTMM_H
#define FXOS_BOOTMM_H

#include <fxos/mm.h>

phys_addr_t bootmm_get_base(void);
phys_addr_t bootmm_get_tail(void);
phys_addr_t bootmm_get_limit(void);

int bootmm_init(phys_addr_t base, size_t limit);
void *bootmm_alloc(size_t len, size_t align);

#endif
