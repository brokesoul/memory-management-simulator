#include "BuddyAllocator.h"
#include <iostream>
#include <iomanip>

BuddyAllocator::BuddyAllocator() : total_size(0), base_addr(0) {}

size_t BuddyAllocator::next_pow2(size_t x) const {
    size_t p = 1;
    while (p < x) p <<= 1;
    return p;
}

void BuddyAllocator::init(size_t size) {
    total_size = next_pow2(size);
    base_addr = 0;
    free_lists.clear();
    allocated.clear();
    free_lists[total_size].insert(base_addr);
}

size_t BuddyAllocator::buddy_of(size_t addr, size_t size) const {
    return addr ^ size;
}

void BuddyAllocator::split_block(size_t size) {
    auto it = free_lists.lower_bound(size << 1);
    if (it == free_lists.end()) return;

    size_t big_size = it->first;
    size_t addr = *it->second.begin();
    it->second.erase(addr);
    if (it->second.empty()) free_lists.erase(it);

    size_t half = big_size >> 1;
    free_lists[half].insert(addr);
    free_lists[half].insert(addr + half);
}

int BuddyAllocator::alloc(size_t size) {
    size_t req = next_pow2(size);

    if (free_lists.find(req) == free_lists.end()) {
        split_block(req);
    }

    auto it = free_lists.find(req);
    if (it == free_lists.end() || it->second.empty()) return -1;

    size_t addr = *it->second.begin();
    it->second.erase(addr);
    if (it->second.empty()) free_lists.erase(it);

    allocated[addr] = req;
    return static_cast<int>(addr);
}

bool BuddyAllocator::free_block(size_t addr) {
    auto it = allocated.find(addr);
    if (it == allocated.end()) return false;

    size_t size = it->second;
    allocated.erase(it);

    size_t curr_addr = addr;
    size_t curr_size = size;

    while (true) {
        size_t buddy = buddy_of(curr_addr, curr_size);
        auto fit = free_lists.find(curr_size);
        if (fit == free_lists.end() ||
            fit->second.find(buddy) == fit->second.end()) {
            break;
        }

        fit->second.erase(buddy);
        if (fit->second.empty()) free_lists.erase(fit);

        curr_addr = std::min(curr_addr, buddy);
        curr_size <<= 1;
    }

    free_lists[curr_size].insert(curr_addr);
    return true;
}

void BuddyAllocator::dump() const {
    std::cout << "Buddy Free Lists:\n";
    for (auto& [sz, addrs] : free_lists) {
        std::cout << "Size " << sz << ": ";
        for (auto a : addrs)
            std::cout << "0x" << std::hex << a << " ";
        std::cout << std::dec << "\n";
    }

    std::cout << "Allocated Blocks:\n";
    for (auto& [a, sz] : allocated)
        std::cout << "[0x" << std::hex << a << "] size=" << std::dec << sz << "\n";
}
