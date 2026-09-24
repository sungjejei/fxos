#include <fxos/mm.h>
#include <fxos/bootmm.h>
#include <fxos/kernel.h>

/*
 * Note:
 *   Buddy Allocator is not implemented yet. Max order is 0!!!
 *   This will use a linear allocator instead.
 *   This can't allocate multiple frames but it's easy to implement.
 *   I'm going to implement a buddy allocator later.
 */

static struct frame *frames = NULL;
static struct frame *free_region[MAX_ORDER + 1] = { NULL };
static page_count_t frames_count = 0;

phys_addr_t get_frame_addr(struct frame *frame)
{
    return (frame - frames) << PAGE_SHIFT;
}

struct frame *get_frame_by_addr(phys_addr_t addr)
{
    return &frames[addr >> PAGE_SHIFT];
}

int init_frames(page_count_t count)
{
    frames_count = count;
    frames = bootmm_alloc(sizeof(struct frame) * frames_count, 8);

    if ((uintptr_t)frames == INVALID_ADDRESS)
        return -1;

    for (page_count_t i = 0; i < count; i++)
    {
        frames[i].refcount = 0;
        frames[i].next = NULL;
        frames[i].order = -1;
        frames[i].flags = FRAME_RESERVED;
    }

    return 0;
}

static void unreserve_frames(page_index_t begin, page_count_t count)
{
    page_index_t limit = begin + count;

    for (; begin < limit; begin++)
        frames[begin].flags &= ~FRAME_RESERVED;
}

static void insert_frame_to_list(struct frame *frame)
{
    frame->next = free_region[0];
    free_region[0] = frame;
}

static struct frame *pop_frame_from_list(void)
{
    struct frame *frame = free_region[0];
    
    if (frame)
        free_region[0] = free_region[0]->next;
    
    return frame;
}

/*
 * Not a buddy allocator.
 */
int init_buddy(page_index_t begin, page_index_t limit)
{
    size_t len = limit - begin;

    unreserve_frames(begin, len);

    for (; begin < limit; begin++)
    {
        frames[begin].order = 0;
        insert_frame_to_list(&frames[begin]);
    }


    return 0;
}

struct frame *alloc_frame(void)
{
    struct frame *frame = pop_frame_from_list();

    if (!frame)
        return NULL;

    frame->refcount++;
    return frame;
}

void free_frame(struct frame *frame)
{
    if (frame->refcount == 0)
        panic("Frame double-free detected! frame=%p addr=0x%X", frame, get_frame_addr(frame));

    frame->refcount--;

    if (frame->refcount == 0)
        insert_frame_to_list(frame);
}
