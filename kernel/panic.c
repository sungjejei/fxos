#include <fxos/cpu.h>
#include <fxos/kernel.h>
#include <fxos/uart.h>
#include <fxos/rtl.h>

static size_t put(char ch, void *data)
{
    (void)data;
    uart_write(ch);
    return 1;
}

void __noreturn vpanic(const char *fmt, va_list args)
{
    uart_write_str("Kernel panic: ");
    vprintfmt(put, NULL, fmt, args);
    uart_write_str("\r\n");
    halt_cpu_forever();
}

void __noreturn panic(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vpanic(fmt, args);
}

