#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include <unordered_map>
#include <cstddef>

struct PageEntry {
    bool valid;
    size_t frame;
};

class VirtualMemory {
public:
    VirtualMemory(size_t page_size, size_t num_frames);

    size_t access(size_t virtual_address);
    void stats() const;

private:
    size_t page_size;
    size_t num_frames;

    std::unordered_map<size_t, PageEntry> page_table;
    std::unordered_map<size_t, size_t> frame_table; // frame -> page

    size_t next_frame;

    size_t page_hits;
    size_t page_faults;
};

#endif
