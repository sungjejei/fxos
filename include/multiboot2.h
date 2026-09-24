#ifndef MULTIBOOT2_H
#define MULTIBOOT2_H

#define MULTIBOOT2_HEADER_MAGIC 0xE85250D6
#define MULTIBOOT2_ARCH_I386    0
#define MULTIBOOT2_LOADER_MAGIC 0x36D76289

#define MULTIBOOT2_TAG_MMAP     6

#ifndef __ASSEMBLER__

#include <fxos/types.h>

struct multiboot2_tag {
    uint32_t type;
    uint32_t size;
};

struct multiboot2_info {
    uint32_t total_size;
    uint32_t reserved;
    struct multiboot2_tag tags[];
};

struct multiboot2_mmap_entry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t reserved;
};

struct multiboot2_mmap {
    struct multiboot2_tag head;
    uint32_t entry_size;
    uint32_t entry_version;
    struct multiboot2_mmap_entry entries[];
};

#endif

#endif
