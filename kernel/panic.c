#include <fxos/cpu.h>
#include <fxos/kernel.h>
#include <fxos/uart.h>
#include <fxos/rtl.h>

void __noreturn panic(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    uart_write_str("kernel panic: ");
    vprintfmt((putchar_routine_t)(void*)uart_write, NULL, fmt, args);
    uart_write('\n');
    va_end(args);
    halt_cpu_forever();
}
