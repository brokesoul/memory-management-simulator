#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <queue>
#include <cstddef>

struct CacheLine {
    bool valid;
    size_t tag;

    CacheLine() : valid(false), tag(0) {}
};

class Cache {
public:
    Cache(size_t cache_size,
          size_t block_size,
          size_t associativity);

    bool access(size_t address); // returns true = hit, false = miss
    void stats() const;

private:
    size_t cache_size;
    size_t block_size;
    size_t associativity;
    size_t num_sets;

    std::vector<std::vector<CacheLine>> sets;
    std::vector<std::queue<size_t>> fifo_queues;

    size_t hits;
    size_t misses;

    size_t get_set_index(size_t address) const;
    size_t get_tag(size_t address) const;
};

#endif
