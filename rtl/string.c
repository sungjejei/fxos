#include <fxos/types.h>
#include <fxos/kernel.h>
#include <fxos/rtl.h>

void *memset(void *dst, int val, size_t len)
{
    void *ret = dst;
    while (len--)
        *(uint8_t*)dst++ = (uint8_t)val;
    return ret;
}

void *memcpy(void *dst, void *src, size_t len)
{
    void *ret = dst;
    while (len--)
        *(uint8_t*)dst++ = *(uint8_t*)src++;
    return ret;
}

void utoa(uint64_t value, char *buffer, unsigned int radix, int lowercase)
{
    if (radix > 16) panic("Invalid argument passed to 'utoa'");

    const char *digits = lowercase ? "0123456789abcdef" : "0123456789ABCDEF";
    char *head = buffer;
    char *tail = buffer;

    while (value)
    {
        *tail++ = digits[value % radix];
        value /= radix;
    }

    *tail-- = '\0';

    while (tail > head)
    {
        char tmp = *tail;
        *tail = *head;
        *head = tmp;
        tail--;
        head++;
    }
}

void itoa(int64_t value, char *buffer, unsigned int radix, int lowercase)
{
    uint64_t uvalue;
    int negative = value < 0;

    if (negative)
    {
        uvalue = ~(uint64_t)value + 1;
        *buffer++ = '-';
    }
    else
    {
        uvalue = value;
    }

    utoa(uvalue, buffer, radix, lowercase);
}
