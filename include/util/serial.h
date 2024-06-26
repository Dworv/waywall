#ifndef WAYWALL_UTIL_SERIAL_H
#define WAYWALL_UTIL_SERIAL_H

#include "util/prelude.h"
#include <stddef.h>
#include <stdint.h>
#include <wayland-server-core.h>

struct serial_ring {
    uint32_t data[64];
    size_t tail, len;
};

static inline int
serial_ring_consume(struct serial_ring *ring, uint32_t serial) {
    for (size_t i = 0; i < ring->len; i++) {
        uint32_t datum = ring->data[(ring->tail + i) % STATIC_ARRLEN(ring->data)];
        if (datum == serial) {
            ring->tail = (ring->tail + i + 1) % STATIC_ARRLEN(ring->data);
            ring->len = ring->len - i - 1;
            return 0;
        }
    }
    return 1;
}

static inline int
serial_ring_push(struct serial_ring *ring, uint32_t serial) {
    if (ring->len == STATIC_ARRLEN(ring->data)) {
        return 1;
    }
    ring->data[(ring->tail + ring->len) % STATIC_ARRLEN(ring->data)] = serial;
    ring->len++;
    return 0;
}

static inline uint32_t
next_serial(struct wl_resource *resource) {
    return wl_display_next_serial(wl_client_get_display(wl_resource_get_client(resource)));
}

#endif
