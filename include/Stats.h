#ifndef STATS_H
#define STATS_H

#include <cstddef>

struct Stats {
    size_t total_memory = 0;
    size_t used_memory = 0;
    size_t requested_memory = 0;

    size_t alloc_requests = 0;
    size_t alloc_failures = 0;

    size_t internal_frag = 0;

    void reset(size_t mem) {
        total_memory = mem;
        used_memory = 0;
        requested_memory = 0;
        alloc_requests = 0;
        alloc_failures = 0;
        internal_frag = 0;
    }
};

#endif
