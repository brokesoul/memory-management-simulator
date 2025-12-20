#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <map>
#include <set>
#include <cstddef>

class BuddyAllocator {
public:
    BuddyAllocator();

    void init(size_t size);
    int alloc(size_t size);
    bool free_block(size_t addr);
    void dump() const;

private:
    size_t total_size;
    size_t base_addr;

    // free_lists[size] = set of starting addresses
    std::map<size_t, std::set<size_t>> free_lists;

    // allocated[address] = block size
    std::map<size_t, size_t> allocated;

    size_t next_pow2(size_t x) const;
    size_t buddy_of(size_t addr, size_t size) const;

    void split_block(size_t size);
};

#endif
