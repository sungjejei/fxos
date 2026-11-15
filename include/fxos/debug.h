#ifndef FXOS_DEBUG_H
#define FXOS_DEBUG_H

#include <fxos/kernel.h>

#ifdef DEBUG
#define DPRINT(fmt, ...) kprintf(fmt, ##__VA_ARGS__)
#else
#define DPRINT(fmt, ...)
#endif

#endif
