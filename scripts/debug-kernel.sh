#!/bin/sh

make isoimage || exit 1

setsid qemu-system-x86_64 -cdrom disk.iso -S -s -display none &
QEMU_PID=$!

gdb -ex "b kmain" -ex "target remote :1234" fxos

kill -TERM -- "-$QEMU_PID"
wait "$QEMU_PID"
