#ifndef FXOS_UART_H
#define FXOS_UART_H

#include <fxos/types.h>

int uart_init(void);
int uart_write(char ch);
int uart_write_str(const char *s);

#endif
