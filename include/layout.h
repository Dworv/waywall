#ifndef WAYWALL_LAYOUT_H
#define WAYWALL_LAYOUT_H

#include "util.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 *  layout contains a list of items to display to the user, such as instances and rectangles.
 */
struct layout {
    struct layout_entry *entries;
    size_t entry_count;

    uint64_t reset_all_ids[INSTANCE_BITFIELD_WIDTH / sizeof(uint64_t)];
};

static_assert(INSTANCE_BITFIELD_WIDTH / sizeof(uint64_t) == 2, "bitfield width size is 2");

/*
 *  layout_entry contains a single item to be displayed to the user.
 */
struct layout_entry {
    enum layout_entry_type {
        INSTANCE,
        RECTANGLE,
    } type;
    int x, y, w, h;
    union {
        int instance;
        float color[4];
    } data;
};

/*
 *  Frees any resources associated with the given layout.
 */
void layout_destroy(struct layout layout);

/*
 *  Cleans up any resources allocated by the layout module.
 */
void layout_fini();

/*
 *  Initializes data used by the layout module.
 */
bool layout_init();

struct wall;

/*
 *  Requests a new layout with the current state of each instance.
 */
struct layout layout_request_new(struct wall *wall);

/*
 *  Attempts to reinitialize the layout generator.
 */
bool layout_reinit();

#endif
