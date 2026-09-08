#include <fxos/kernel.h>
#include <fxos/cpu.h>
#include <fxos/uart.h>

extern void *isr_table[IDT_COUNT];

void init_interrupt(void)
{
    uint8_t attr = IDT_ATTRIBUTE_TYPE_INTERRUPT_GATE | IDT_ATTRIBUTE_PRESENT | IDT_ATTRIBUTE_PRIVILEGE_KERNELONLY;

    init_idt();

    for (unsigned int i = 0; i < IDT_COUNT; i++)
    {
        set_idte(i, isr_table[i], attr, 0);
    }
}

void __noreturn interrupt_service(struct trap_frame *frame)
{
    panic("unhandled exception: %u", frame->vector);
}
