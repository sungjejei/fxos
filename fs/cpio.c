#include <fxos/rtl.h>
#include <fxos/kernel.h>
#include <fxos/fs/cpio.h>

static const char newc_magic[] = "070701";
static const char newc_end[] = "TRAILER!!!";

static uint32_t parse(const char *s)
{
    uint32_t value = 0;

    for (int i = 0; i < 8; i++) {
        if (s[i] >= '0' && s[i] <= '9')
            value = value * 16 + s[i] - '0';
        else if (s[i] >= 'A' && s[i] <= 'F')
            value = value * 16 + s[i] - 'A' + 10;
        else if (s[i] >= 'a' && s[i] <= 'f')
            value = value * 16 + s[i] - 'a' + 10;
        else
            break;
    }

    return value;
}

static inline int is_valid(const struct cpio_header *header)
{
    return strncmp(header->magic, newc_magic, 6) == 0;
}

static inline int is_end(const struct cpio_header *header)
{
    return strncmp(header->name, newc_end, sizeof(newc_end) - 1) == 0;
}

const struct cpio_header *cpio_next(const struct cpio_header *header, void *limit)
{
    if (!is_valid(header) || is_end(header))
        return NULL;

    const struct cpio_header *next = (const struct cpio_header *)(
        ALIGN_UP((uintptr_t)header + sizeof(*header) + parse(header->namesize), 4) +
        ALIGN_UP(parse(header->filesize), 4)
    );

    if ((uintptr_t)next + sizeof(*next) + parse(header->namesize) >= (uintptr_t)limit)
        return NULL;

    if ((uintptr_t)next >= (uintptr_t)limit)
        return NULL;

    return next;
}

const struct cpio_header *cpio_find(const struct cpio_header *header, size_t len, const char *name)
{
    void *limit = (void*)((uintptr_t)header + len);

    while (header) {
        size_t namelen = parse(header->namesize);
        
        if ((uintptr_t)header + sizeof(*header) + namelen >= (uintptr_t)limit)
            return NULL;

        if (strncmp(header->name, name, namelen) == 0)
            return header;

        header = cpio_next(header, limit);
    }

    return NULL;
}
