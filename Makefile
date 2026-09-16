#
# Top level Makefile for fxos
#

# Make Variables
CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
AS := $(CC)
AR := $(CROSS_COMPILE)ar
QEMU := qemu-system-x86_64

# Make Flags
CFLAGS := -ffreestanding -fno-builtin -nostdlib -fno-pic \
          -fno-stack-protector -mno-red-zone -fno-pie \
          -fno-omit-frame-pointer -fcf-protection=none \
          -mgeneral-regs-only -mno-sse -mno-sse2 -mno-mmx -mno-avx \
          -mcmodel=kernel -c -Iinclude -g -Og -MMD -MP \
          -Wall -Wextra
LDFLAGS := -T scripts/linker.ld
ASFLAGS := $(CFLAGS)
QEMUFLAGS := -serial stdio -d int -D qemu.log -no-reboot
ARFLAGS := 

# Subdirectories
SUBDIRECTORIES := boot/ cpu/ drivers/ init/ kernel/ rtl/ mm/

# Echoes
ifeq ($(V), 1)
Q :=
else
Q := @
endif

# Exports
export CC LD AS AR
export CFLAGS LDFLAGS ASFLAGS ARFLAGS
export Q

# PHONY
.PHONY: all clean iso run FORCE

# Build
all: fxos

# Clean
clean:
	$(Q)for i in $(SUBDIRECTORIES); do \
		$(MAKE) -f scripts/Makefile.build TARGET_SUBDIRECTORY=$$i clean --no-print-directory; \
	done
	@echo "  CLEAN     /tmp/fxos-build/"
	$(Q)rm -rf /tmp/fxos-build
	@echo "  CLEAN     ."
	$(Q)rm -f disk.iso fxos

# ISO
iso: disk.iso
disk.iso: fxos
	@echo "  ISO       $@"
	$(Q)mkdir -p /tmp/fxos-build
	$(Q)mkdir -p /tmp/fxos-build/boot/grub
	$(Q)cp $< /tmp/fxos-build/boot/fxos
	$(Q)cp scripts/grub.cfg /tmp/fxos-build/boot/grub/grub.cfg
	$(Q)grub-mkrescue -o $@ /tmp/fxos-build > /dev/null 2> /dev/null

# Run
run: disk.iso
	@echo "  RUN"
	$(Q)$(QEMU) $(QEMUFLAGS) -cdrom $<

# fxos
fxos: $(addsuffix built-in.a, $(SUBDIRECTORIES))
	@echo "  LD        $@"
	$(Q)$(LD) $(LDFLAGS) --start-group $^ --end-group -o $@

# Subdirectory build rules
%/built-in.a: FORCE
	$(Q)$(MAKE) -f scripts/Makefile.build TARGET_SUBDIRECTORY=$*/ --no-print-directory
