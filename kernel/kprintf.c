#include <fxos/uart.h>
#include <fxos/kernel.h>
#include <fxos/rtl.h>

static size_t put(char ch, void *param)
{
    (void)param;
    uart_write(ch);
    return 1;
}

size_t kprintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    size_t ret = vprintfmt(put, NULL, fmt, args);
    va_end(args);
    return ret;
}
