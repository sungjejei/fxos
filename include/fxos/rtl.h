#ifndef FXOS_RTL_H
#define FXOS_RTL_H

#include <fxos/types.h>
#include <stdarg.h>

typedef void (*putchar_routine_t)(char ch, void *data);

void *memset(void *dst, int val, size_t len);
void *memcpy(void *dst, void *src, size_t len);
void utoa(uint64_t value, char *buffer, unsigned int radix, int lowercase);
void itoa(int64_t value, char *buffer, unsigned int radix, int lowercase);

size_t printfmt(putchar_routine_t put, void *put_param, const char *fmt, ...);
size_t vprintfmt(putchar_routine_t put, void* put_param, const char *fmt, va_list args);

uint64_t roundup_power_of_two(uint64_t);
uint8_t msb_index(uint64_t);

#endif
