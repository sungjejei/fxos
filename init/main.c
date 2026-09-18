#include <fxos/types.h>
#include <fxos/uart.h>
#include <fxos/cpu.h>
#include <fxos/kernel.h>
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

struct multiboot2_mmap *mmap;

static void parse_loader_info(struct multiboot2_info *info)
{
    struct multiboot2_tag *tag = info->tags;

    kprintf("multiboot2: GRUB-provided informations\n");

    while (tag->type != 0)
    {
        kprintf("multiboot2-tag: type=%u <%s> size=%u\n", tag->type, multiboot2_tag_names[tag->type], tag->size);
        tag = (struct multiboot2_tag*)ALIGN_UP((uintptr_t)tag + tag->size, 8);
    }
}

void kmain(uint32_t magic, struct multiboot2_info *info)
{
    (void)magic;(void)info;
    
    init_gdt();
    init_idt();
    init_interrupt();

    kprintf("Hello, kernel!\nmagic=0x%X info=%p\n", magic, info);

    parse_loader_info(info);

    halt_cpu_forever();
}
