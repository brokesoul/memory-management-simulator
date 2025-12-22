#include "VirtualMemory.h"
#include <iostream>

VirtualMemory::VirtualMemory(size_t vsize,
                             size_t psize,
                             size_t psize_page)
    : virt_size(vsize),
      phys_size(psize),
      page_size(psize_page),
      page_hits(0),
      page_faults(0)
{
    num_pages = virt_size / page_size;
    num_frames = phys_size / page_size;

    page_table.resize(num_pages);
    frame_used.resize(num_frames, false);

    for (auto &e : page_table) {
        e.valid = false;
        e.frame = 0;
    }
}

size_t VirtualMemory::get_page(size_t vaddr) const {
    return vaddr / page_size;
}

size_t VirtualMemory::get_offset(size_t vaddr) const {
    return vaddr % page_size;
}

bool VirtualMemory::access(size_t vaddr, size_t &paddr) {
    size_t page = get_page(vaddr);
    size_t offset = get_offset(vaddr);

    if (page >= num_pages)
        return false;

    if (page_table[page].valid) {
        page_hits++;
        paddr = page_table[page].frame * page_size + offset;
        return true;
    }

    /* PAGE FAULT */
    page_faults++;

    size_t frame = num_frames;
    for (size_t i = 0; i < num_frames; i++) {
        if (!frame_used[i]) {
            frame = i;
            break;
        }
    }

    /* FIFO REPLACEMENT */
    if (frame == num_frames) {
        size_t victim_page = fifo_pages.front();
        fifo_pages.pop();

        frame = page_table[victim_page].frame;
        page_table[victim_page].valid = false;
    }

    page_table[page].valid = true;
    page_table[page].frame = frame;
    frame_used[frame] = true;

    fifo_pages.push(page);

    paddr = frame * page_size + offset;
    return false;
}

void VirtualMemory::stats() const {
    std::cout << "Page hits: " << page_hits << "\n";
    std::cout << "Page faults: " << page_faults << "\n";
}

