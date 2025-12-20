#include "VirtualMemory.h"
#include <iostream>

VirtualMemory::VirtualMemory(size_t psize, size_t frames)
    : page_size(psize),
      num_frames(frames),
      next_frame(0),
      page_hits(0),
      page_faults(0) {}

size_t VirtualMemory::access(size_t vaddr) {
    size_t page = vaddr / page_size;
    size_t offset = vaddr % page_size;

    auto it = page_table.find(page);
    if (it != page_table.end() && it->second.valid) {
        page_hits++;
        return it->second.frame * page_size + offset;
    }

    page_faults++;

    size_t frame;
    if (next_frame < num_frames) {
        frame = next_frame++;
    } else {
        // simple FIFO eviction
        auto evict = frame_table.begin();
        size_t evict_frame = evict->first;
        size_t evict_page = evict->second;
        page_table[evict_page].valid = false;
        frame = evict_frame;
        frame_table.erase(evict);
    }

    page_table[page] = {true, frame};
    frame_table[frame] = page;

    return frame * page_size + offset;
}

void VirtualMemory::stats() const {
    std::cout << "Page hits: " << page_hits << "\n";
    std::cout << "Page faults: " << page_faults << "\n";
}
