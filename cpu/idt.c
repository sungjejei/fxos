#include <fxos/cpu.h>
#include <fxos/rtl.h>
#include <fxos/kernel.h>

struct idte idt[IDT_COUNT];

void init_idt(void)
{
    memset(&idt, 0, sizeof(idt));
    struct idtr idtr = { sizeof(idt) - 1, (uintptr_t)&idt };
    __asm__ volatile ("lidt %0"::"m"(idtr));
}

void set_idte(unsigned int index, void *routine, uint8_t attr, uint8_t ist)
{
    if (index >= IDT_COUNT) panic("Invalid argument for 'std_idte()'");

    idt[index].attributes = attr;
    idt[index].ist = ist;
    idt[index].selector = KERNEL_CODE_SELECTOR;
    idt[index].offset_low = (uintptr_t)routine & 0xFFFF;
    idt[index].offset_mid = ((uintptr_t)routine >> 16) & 0xFFFF;
    idt[index].offset_high = ((uintptr_t)routine >> 32) & 0xFFFFFFFF;
}
