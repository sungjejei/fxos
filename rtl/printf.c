#include <fxos/rtl.h>
#include <stdarg.h>

static void write_str(putchar_routine_t routine, void *put_param, const char *s)
{
    while (*s)
    {
        routine(*s, put_param);
        s++;
    }
}

size_t vprintfmt(putchar_routine_t put, void *put_param, const char *fmt, va_list args)
{
    const char *tmp = fmt;
    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;

            switch (*fmt)
            {
            case 's':
            {
                const char *s = va_arg(args, const char *);
                if (!s) write_str(put, put_param, "<null>");
                else write_str(put, put_param, s);
                break;
            }
            case 'd':
            {
                char buffer[65];
                itoa(va_arg(args, int64_t), buffer, 10, 0);
                write_str(put, put_param, buffer);
                break;
            }
            case 'u':
            {
                char buffer[65];
                utoa(va_arg(args, uint64_t), buffer, 10, 0);
                write_str(put, put_param, buffer);
                break;
            }
            case 'x':
            {
                char buffer[65];
                utoa(va_arg(args, uint64_t), buffer, 16, 1);
                write_str(put, put_param, buffer);
                break;
            }
            case 'X':
            {
                char buffer[65];
                utoa(va_arg(args, uint64_t), buffer, 16, 0);
                write_str(put, put_param, buffer);
                break;
            }
            case 'p':
            {
                char buffer[65];
                utoa(va_arg(args, uint64_t), buffer, 16, 0);
                write_str(put, put_param, "0x");
                write_str(put, put_param, buffer);
                break;
            }
            case '%':
            {
                put('%', put_param);
                break;
            }
            }

            fmt++;
        }
        else
        {
            put(*fmt, put_param);
            fmt++;
        }
    }
    return fmt - tmp;
}

size_t printfmt(putchar_routine_t put, void *put_param, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    size_t ret = vprintfmt(put, put_param, fmt, args);
    va_end(args);
    return ret;
}
