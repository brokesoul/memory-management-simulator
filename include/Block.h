#ifndef BLOCK_H
#define BLOCK_H

#include <cstddef>

struct Block {
    size_t start;      // start address
    size_t size;       // size in bytes
    bool free;         // free or used
    int id;            // allocation id (-1 if free)

    Block* prev;
    Block* next;

    Block(size_t s, size_t sz, bool f, int bid)
        : start(s), size(sz), free(f), id(bid),
          prev(nullptr), next(nullptr) {}
};

#endif
