#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <cstddef>
#include <map>
#include <set>

class BuddyAllocator {
public:
    BuddyAllocator();
    void init(size_t size);
    int malloc_block(size_t size);
    bool free_block(int addr);
    void dump() const;

private:
    size_t total_size;
    size_t min_order;
    size_t max_order;

    std::map<size_t, std::set<size_t>> free_lists;
    std::map<size_t, size_t> allocated; // addr -> order

    size_t next_power_of_two(size_t x) const;
    size_t order_of(size_t size) const;
    size_t buddy_of(size_t addr, size_t order) const;
};

#endif
