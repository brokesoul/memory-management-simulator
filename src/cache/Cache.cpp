#include "Cache.h"
#include <iostream>

Cache::Cache(size_t csize, size_t bsize, size_t assoc)
    : cache_size(csize),
      block_size(bsize),
      associativity(assoc),
      hits(0),
      misses(0)
{
    num_sets = cache_size / (block_size * associativity);
    sets.resize(num_sets);
}

bool Cache::access(size_t addr) {
    size_t block_addr = addr / block_size;
    size_t set_idx = block_addr % num_sets;
    size_t tag = block_addr / num_sets;

    auto& set = sets[set_idx];

    for (auto& line : set) {
        if (line.valid && line.tag == tag) {
            hits++;
            return true;
        }
    }

    misses++;

    if (set.size() >= associativity) {
        set.pop_front(); // FIFO eviction
    }

    CacheLine newline;
    newline.valid = true;
    newline.tag = tag;
    set.push_back(newline);

    return false;
}

void Cache::stats() const {
    std::cout << "Cache hits: " << hits << "\n";
    std::cout << "Cache misses: " << misses << "\n";
}
