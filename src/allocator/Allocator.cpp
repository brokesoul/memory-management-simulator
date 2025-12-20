#include "Allocator.h"
#include <iostream>
#include <iomanip>

Allocator::Allocator() : head(nullptr), policy(AllocPolicy::FIRST_FIT), next_id(1) {}

Allocator::~Allocator() {
    Block* cur = head;
    while (cur) {
        Block* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
}

void Allocator::init(size_t size) {
    if (head) {
        this->~Allocator();
        head = nullptr;
    }
    head = new Block(0, size, true, -1);
    policy = AllocPolicy::FIRST_FIT;
    next_id = 1;
    stats_data.reset(size);
}

void Allocator::set_policy(AllocPolicy p) {
    policy = p;
}

Block* Allocator::find_block(size_t size) {
    Block* best = nullptr;
    for (Block* cur = head; cur; cur = cur->next) {
        if (!cur->free || cur->size < size) continue;

        if (policy == AllocPolicy::FIRST_FIT)
            return cur;

        if (!best) best = cur;
        else if (policy == AllocPolicy::BEST_FIT && cur->size < best->size)
            best = cur;
        else if (policy == AllocPolicy::WORST_FIT && cur->size > best->size)
            best = cur;
    }
    return best;
}

void Allocator::split_block(Block* blk, size_t size) {
    if (blk->size == size) return;

    Block* newblk = new Block(
        blk->start + size,
        blk->size - size,
        true,
        -1
    );

    newblk->next = blk->next;
    if (newblk->next) newblk->next->prev = newblk;
    blk->next = newblk;
    newblk->prev = blk;

    blk->size = size;
}

size_t Allocator::malloc_block(size_t size, int &out_id) {
    stats_data.alloc_requests++;

    Block* blk = find_block(size);
    if (!blk) {
        stats_data.alloc_failures++;
        out_id = -1;
        return static_cast<size_t>(-1);
    }

    split_block(blk, size);

    blk->free = false;
    blk->id = next_id++;
    out_id = blk->id;

    stats_data.used_memory += blk->size;
    stats_data.requested_memory += size;
    stats_data.internal_frag += (blk->size - size);

    return blk->start;
}

void Allocator::coalesce(Block* blk) {
    if (blk->prev && blk->prev->free) {
        blk->prev->size += blk->size;
        blk->prev->next = blk->next;
        if (blk->next) blk->next->prev = blk->prev;
        delete blk;
        blk = blk->prev;
    }

    if (blk->next && blk->next->free) {
        Block* nxt = blk->next;
        blk->size += nxt->size;
        blk->next = nxt->next;
        if (nxt->next) nxt->next->prev = blk;
        delete nxt;
    }
}

bool Allocator::free_block(int id) {
    for (Block* cur = head; cur; cur = cur->next) {
        if (!cur->free && cur->id == id) {
            cur->free = true;
            cur->id = -1;
            stats_data.used_memory -= cur->size;
            coalesce(cur);
            return true;
        }
    }
    return false;
}

void Allocator::dump() const {
    for (Block* cur = head; cur; cur = cur->next) {
        std::cout << "[0x" << std::hex << std::setw(4) << std::setfill('0')
                  << cur->start << " - 0x"
                  << std::setw(4) << (cur->start + cur->size - 1)
                  << "] ";
        if (cur->free) std::cout << "FREE\n";
        else std::cout << "USED (id=" << std::dec << cur->id << ")\n";
    }
}

void Allocator::stats() const {
    std::cout << "Total memory: " << stats_data.total_memory << "\n";
    std::cout << "Used memory: " << stats_data.used_memory << "\n";
    double ext_frag = 0.0;
    size_t free_total = stats_data.total_memory - stats_data.used_memory;
    size_t max_free = 0;
    for (Block* cur = head; cur; cur = cur->next)
        if (cur->free && cur->size > max_free) max_free = cur->size;
    if (free_total > 0)
        ext_frag = 1.0 - (double)max_free / free_total;
    std::cout << "External fragmentation: " << ext_frag * 100 << "%\n";
}
