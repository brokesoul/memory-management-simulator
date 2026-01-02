#include "Cache.h"
#include <iostream>

Cache::Cache(size_t c_size,
             size_t b_size,
             size_t assoc)
    : cache_size(c_size),
      block_size(b_size),
      associativity(assoc),
      hits(0),
      misses(0)
{
    num_sets = cache_size / (block_size * associativity);

    sets.resize(num_sets);
    fifo_queues.resize(num_sets);

    for (size_t i = 0; i < num_sets; i++) {
        sets[i].resize(associativity);
    }
}

size_t Cache::get_set_index(size_t address) const {
    size_t block_addr = address / block_size;
    return block_addr % num_sets;
}

size_t Cache::get_tag(size_t address) const {
    size_t block_addr = address / block_size;
    return block_addr / num_sets;
}

bool Cache::access(size_t address) {
    size_t set_idx = get_set_index(address);
    size_t tag = get_tag(address);

    auto &set = sets[set_idx];
    auto &fifo = fifo_queues[set_idx];

    //HIT CHECK 
    for (auto &line : set) {
        if (line.valid && line.tag == tag) {
            hits++;
            return true;
        }
    }


    misses++;

    for (size_t i = 0; i < associativity; i++) {
        if (!set[i].valid) {
            set[i].valid = true;
            set[i].tag = tag;
            fifo.push(i);
            return false;
        }
    }


    size_t victim = fifo.front();
    fifo.pop();

    set[victim].tag = tag;
    fifo.push(victim);

    return false;
}

void Cache::stats() const {
    std::cout << "Cache hits: " << hits << "\n";
    std::cout << "Cache misses: " << misses << "\n";
}
