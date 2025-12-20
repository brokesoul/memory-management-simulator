#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <deque>
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

    bool access(size_t physical_address);
    void stats() const;

private:
    size_t cache_size;
    size_t block_size;
    size_t associativity;
    size_t num_sets;

    std::vector<std::deque<CacheLine>> sets;

    size_t hits;
    size_t misses;
};

#endif
