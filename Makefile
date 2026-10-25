#
# Top level Makefile for FXOS
#

SUBDIRS := boot/ cpu/ drivers/ init/ kernel/ mm/ rtl/

LD := ld
LDFLAGS := -T scripts/linker.ld
QEMU := qemu-system-x86_64
QEMUFLAGS := -enable-kvm -cpu host -serial stdio

ifeq ($(QEMU_ENABLE_DISPLAY), 1)
QEMUFLAGS +=
else
QEMUFLAGS += -display none
endif

ifeq ($(V), 1)
Q :=
else
Q := @
endif

.PHONY: FORCE all clean iso isoimage

all: fxos

fxos: $(addsuffix built-in.a, $(SUBDIRS))
	$(Q)echo "  LD        fxos"
	$(Q)$(LD) $(LDFLAGS) --start-group $^ --end-group -o fxos

%/built-in.a: FORCE
	$(Q)$(MAKE) -f scripts/Makefile.build --no-print-directory TARGET_SUBDIR=$*/ all

clean:
	$(Q)for i in $(SUBDIRS); do \
		$(MAKE) -f scripts/Makefile.build --no-print-directory TARGET_SUBDIR=$$i clean; \
	done
	$(Q)echo "  CLEAN     ."
	$(Q)rm -f fxos disk.iso

iso: disk.iso
isoimage: disk.iso

disk.iso: fxos
	$(Q)echo "  ISO       disk.iso"
	$(Q)scripts/mkisoimage.sh

run: disk.iso
	$(Q)echo "  RUN"
	$(Q)qemu-system-x86_64 $(QEMUFLAGS) -cdrom disk.iso
