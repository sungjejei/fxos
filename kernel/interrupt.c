#include <fxos/kernel.h>
#include <fxos/cpu.h>
#include <fxos/uart.h>

extern void *isr_table[IDT_COUNT];

void init_interrupt(void)
{
    uint8_t attr = IDT_ATTRIBUTE_TYPE_INTERRUPT_GATE | IDT_ATTRIBUTE_PRESENT | IDT_ATTRIBUTE_PRIVILEGE_KERNELONLY;

    init_idt();

    for (unsigned int i = 0; i < IDT_COUNT; i++)
        set_idte(i, isr_table[i], attr, 0);
}

static void __noreturn page_fault(struct trap_frame *frame)
{
    (void)frame;
    kprintf("Page fault ocurred! address=%p\n", read_cr2());
    panic("unhandled page fault");
}

static void __noreturn general_protection(struct trap_frame *frame)
{
    kprintf("General protection fault occured! error_code=%u\n", frame->errcode);
    panic("unhandled general protection fault");
}

static void __noreturn division_error(struct trap_frame *frame)
{
    (void)frame;
    kprintf("Division error occured!");
    panic("unhandled division error");
}

void __noreturn interrupt_service(struct trap_frame *frame)
{
    switch (frame->vector) {
    case CPU_EXCEPTION_DIVIDE_ERROR:
        division_error(frame);
    case CPU_EXCEPTION_GENERAL_PROTECTION:
        general_protection(frame);
    case CPU_EXCEPTION_PAGE_FAULT:
        page_fault(frame);
    default:
        panic("unhandled interrupt occured. vector=%u", frame->vector);
    }
}
