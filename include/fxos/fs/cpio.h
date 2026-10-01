#ifndef FXOS_CPIO_H
#define FXOS_CPIO_H

#include <fxos/types.h>

struct cpio_header {
    char magic[6];
    char ino[8];
    char mode[8];
    char uid[8];
    char gid[8];
    char nlink[8];
    char mtime[8];
    char filesize[8];
    char devmajor[8];
    char devminor[8];
    char rdevmajor[8];
    char rdevminor[8];
    char namesize[8];
    char check[8];
    char name[];
};

const struct cpio_header *cpio_next(const struct cpio_header *header, void *limit);
const struct cpio_header *cpio_find(const struct cpio_header *header, size_t len, const char *name);

#endif
