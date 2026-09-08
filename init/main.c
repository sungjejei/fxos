#include <fxos/types.h>
#include <fxos/uart.h>
#include <fxos/cpu.h>
#include <fxos/kernel.h>
#include <fxos/rtl.h>
#include <multiboot2.h>

static void printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintfmt((putchar_routine_t)(void*)uart_write, NULL, fmt, args);
    va_end(args);
}

void kmain(uint32_t magic, void *info)
{
    (void)magic;(void)info;
    
    init_gdt();
    init_idt();
    init_interrupt();

    printf("Hello, kernel!\nmagic=0x%X info=%p\n", magic, info);

    halt_cpu_forever();
}
