#ifndef FXOS_TYPES_H
#define FXOS_TYPES_H

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

typedef long intptr_t;
typedef unsigned long uintptr_t;

typedef uintptr_t size_t;
typedef intptr_t ssize_t;

#ifndef NULL
#define NULL ((void*)0)
#endif

#define __forceinline __attribute__((always_inline))
#define __noreturn __attribute__((noreturn))

#define ALIGN_UP(value, align) (((uintptr_t)value + (uintptr_t)(align - 1)) & ~(uintptr_t)(align - 1))

#endif
