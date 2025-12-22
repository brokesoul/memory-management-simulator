#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include <cstddef>
#include <vector>
#include <queue>

struct PageTableEntry {
    bool valid;
    size_t frame;
};

class VirtualMemory {
public:
    VirtualMemory(size_t virt_size,
                  size_t phys_size,
                  size_t page_size);

    bool access(size_t virtual_address, size_t &physical_address);

    void stats() const;

private:
    size_t virt_size;
    size_t phys_size;
    size_t page_size;

    size_t num_pages;
    size_t num_frames;

    std::vector<PageTableEntry> page_table;
    std::vector<bool> frame_used;
    std::queue<size_t> fifo_pages;

    size_t page_hits;
    size_t page_faults;

    size_t get_page(size_t vaddr) const;
    size_t get_offset(size_t vaddr) const;
};

#endif
