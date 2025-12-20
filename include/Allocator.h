#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "Block.h"
#include "Stats.h"
#include <string>

enum class AllocPolicy {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
};

class Allocator {
public:
    Allocator();
    ~Allocator();

    void init(size_t size);
    void set_policy(AllocPolicy p);

    int malloc_block(size_t size);
    bool free_block(int id);

    void dump() const;
    void stats() const;

private:
    Block* head;
    AllocPolicy policy;
    Stats stats_data;
    int next_id;

    Block* find_block(size_t size);
    void split_block(Block* blk, size_t size);
    void coalesce(Block* blk);
};

#endif
