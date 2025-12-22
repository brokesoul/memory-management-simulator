#include "BuddyAllocator.h"
#include <iostream>
#include <cmath>

BuddyAllocator::BuddyAllocator()
    : total_size(0), min_order(0), max_order(0) {}

size_t BuddyAllocator::next_power_of_two(size_t x) const {
    size_t p = 1;
    while (p < x) p <<= 1;
    return p;
}

size_t BuddyAllocator::order_of(size_t size) const {
    size_t order = 0;
    size_t s = 1;
    while (s < size) {
        s <<= 1;
        order++;
    }
    return order;
}

void BuddyAllocator::init(size_t size) {
    total_size = next_power_of_two(size);
    free_lists.clear();
    allocated.clear();

    max_order = order_of(total_size);
    min_order = 0;

    free_lists[max_order].insert(0);
}

int BuddyAllocator::malloc_block(size_t size) {
    size_t req_size = next_power_of_two(size);
    size_t order = order_of(req_size);

    size_t cur = order;
    while (cur <= max_order && free_lists[cur].empty())
        cur++;

    if (cur > max_order)
        return -1;

    size_t addr = *free_lists[cur].begin();
    free_lists[cur].erase(addr);

    while (cur > order) {
        cur--;
        size_t buddy = addr + (1ULL << cur);
        free_lists[cur].insert(buddy);
    }

    allocated[addr] = order;
    return static_cast<int>(addr);
}

size_t BuddyAllocator::buddy_of(size_t addr, size_t order) const {
    return addr ^ (1ULL << order);
}

bool BuddyAllocator::free_block(int addr) {
    auto it = allocated.find(addr);
    if (it == allocated.end())
        return false;

    size_t order = it->second;
    allocated.erase(it);

    size_t cur_addr = addr;

    while (order < max_order) {
        size_t buddy = buddy_of(cur_addr, order);
        auto& fl = free_lists[order];

        if (fl.find(buddy) == fl.end())
            break;

        fl.erase(buddy);
        cur_addr = std::min(cur_addr, buddy);
        order++;
    }

    free_lists[order].insert(cur_addr);
    return true;
}

void BuddyAllocator::dump() const {
    std::cout << "Buddy Free Lists:\n";
    for (auto& [order, blocks] : free_lists) {
        if (blocks.empty()) continue;
        std::cout << "Order " << order << " (size "
                  << (1ULL << order) << "): ";
        for (auto addr : blocks)
            std::cout << addr << " ";
        std::cout << "\n";
    }
}
