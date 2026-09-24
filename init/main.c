#include <fxos/types.h>
#include <fxos/uart.h>
#include <fxos/cpu.h>
#include <fxos/kernel.h>
#include <fxos/mm.h>
#include <fxos/rtl.h>
#include <multiboot2.h>

char *multiboot2_tag_names[] = {
    NULL,
    "Boot command line",
    "Bootloader name",
    "Modules",
    "Basic memory information",
    "BIOS boot device",
    "Memory map",
    "VBE",
    "Framebuffer",
    "ELF sections",
    "APM",
    "EFI 32-bit system table",
    "EFI 64-bit system table",
    "SMBIOS information",
    "ACPI RSDP 1.0",
    "ACPI RSDP 2.0",
    "Network configurations",
    "EFI memory map",
    "EFI boot services",
    "EFI 32-bit image handle",
    "EFI 64-bit image handle",
    "Load base address"
};

uint64_t loader_info[4096];
struct multiboot2_mmap *mmap;

static void parse_loader_info(struct multiboot2_info *info)
{
    if (info->total_size >= sizeof(loader_info))
        panic("Loader information parsing failed: too big");

    memcpy(loader_info, info, info->total_size);
    info = (struct multiboot2_info *)loader_info;

    struct multiboot2_tag *tag = info->tags;

    kprintf("multiboot2: GRUB-provided informations\n");

    while (tag->type != 0)
    {
        kprintf("multiboot2-tag: type=%u <%s> size=%u\n", tag->type, multiboot2_tag_names[tag->type], tag->size);

        if (tag->type == MULTIBOOT2_TAG_MMAP)
            mmap = (struct multiboot2_mmap*)tag;
        
        tag = (struct multiboot2_tag*)ALIGN_UP((uintptr_t)tag + tag->size, 8);
    }
}

static void parse_mmap(struct multiboot2_mmap *mmap)
{
    size_t n = (mmap->head.size - sizeof(mmap->head)) / mmap->entry_size;

    kprintf("multiboot2: GRUB-provided memory map\n");

    for (size_t i = 0; i < n; i++)
    {
        kprintf("multiboot2-mmap: [%p-%p] %u\n",
                mmap->entries[i].addr,
                mmap->entries[i].addr + mmap->entries[i].len - 1,
                mmap->entries[i].type);
    }
}

void kmain(uint32_t magic, struct multiboot2_info *info)
{
    if (magic != MULTIBOOT2_LOADER_MAGIC)
        panic("fxos must be booted by GRUB");

    kprintf("fxos kernel magic=0x%X info=%p\n", magic, info);
    
    init_gdt();
    init_idt();
    init_interrupt();

    parse_loader_info(info);
    parse_mmap(mmap);

    mm_init(mmap);

    halt_cpu_forever();
}
