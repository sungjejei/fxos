# FXOS

A Linux-like operating system kernel built as a hobby project.

## What is FXOS?

FXOS is a x86-64 hobby operating system kernel inspired by the Linux architecture.

## Buildsystem of FXOS

Kbuild-like buildsystem is supported.  
To build the kernel:

```
make
```

To create a bootable ISO image contains the kernel:

```
make iso
```

To run the system in QEMU:

```
make run
```

## Architecture support

- x86-64 architecture is supported.  
- Other architectures are not supported yet.

## Features

- [x] Kernel bootstrapping
- [x] Interrupt handling
- [x] Physical frame management
- [ ] Virtual memory manager
- [ ] Filesystem support
- [ ] Scheduling
- [ ] Userspace

## Prerequisites

- GRUB tools - Bootloader image creator (`grub-mkrescue`)
- GNU build utils - Compiler and binary utilities (`gcc`, `ar`, etc.)
- QEMU - Emulator to run the system (`qemu-system-x86_64`)
- GNU make - Kernel buildsystem (`make`)
