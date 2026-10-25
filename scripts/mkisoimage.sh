#!/bin/sh

set -u

ISODIR=$(mktemp -d)
LOGFILE=$(mktemp)
trap 'rm -r "$ISODIR" "$LOGFILE"' EXIT HUP INT TERM

mkdir -p "$ISODIR/boot/grub"

if [ ! -f fxos ]; then
    echo "$0: error: 'fxos' does not exist. Build the kernel first." >&2
    exit 1
fi

cp fxos "$ISODIR/boot/fxos"

cat >"$ISODIR/boot/grub/grub.cfg" <<'EOF'
multiboot2 /boot/fxos
boot
EOF

grub-mkrescue -o disk.iso -quiet "$ISODIR" 2>"$LOGFILE"

if [ "$?" != "0" ]; then
    echo "$0: error: image generation failed." >&2
    cat "$LOGFILE" >&2
    exit 1
fi
