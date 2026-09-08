#include <fxos/types.h>
#include <fxos/cpu.h>
#include <fxos/uart.h>

#define COM1 0x3F8
#define COM2 0x2F8
#define COM3 0x3E8
#define COM4 0x2E8

#define COM  COM1

int uart_init(void)
{
    outb(COM + 1, 0x00);
    outb(COM + 3, 0x80);
    outb(COM + 0, 0x01);
    outb(COM + 1, 0x00);
    outb(COM + 3, 0x03);
    outb(COM + 2, 0xC7);
    outb(COM + 4, 0x0B);
    return 0;
}

int uart_write(char ch)
{
    while (!(inb(COM + 5) & 0x20));
    outb(COM, (uint8_t)ch);
    return 0;
}

int uart_write_str(const char *s)
{
    while (*s)
    {
        uart_write(*s++);
    }
    return 0;
}
