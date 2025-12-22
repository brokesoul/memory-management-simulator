#include <iostream>
#include <sstream>
#include <string>
#include "Cache.h"
#include "Allocator.h"
#include "BuddyAllocator.h"
#include "VirtualMemory.h"
int main() {
    Allocator alloc;
    BuddyAllocator buddy;
Cache *L2 = nullptr;
Cache *L1 = nullptr;
VirtualMemory *vm = nullptr;
    bool buddy_mode = false;

    std::string line;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "exit") {
            break;
        }
else if (cmd == "init_cache") {
    size_t l1_size, l1_block, l1_assoc;
    size_t l2_size, l2_block, l2_assoc;

    ss >> l1_size >> l1_block >> l1_assoc
       >> l2_size >> l2_block >> l2_assoc;

    delete L1;
    delete L2;

    L1 = new Cache(l1_size, l1_block, l1_assoc);
    L2 = new Cache(l2_size, l2_block, l2_assoc);

    std::cout << "L1 and L2 cache initialized\n";
}
else if (cmd == "access") {
    size_t paddr;
    ss >> std::hex >> paddr >> std::dec;

    if (!L1 || !L2) {
        std::cout << "Cache not initialized\n";
        continue;
    }

    if (L1->access(paddr)) {
        std::cout << "L1 HIT\n";
    } else {
        std::cout << "L1 MISS\n";
        if (L2->access(paddr)) {
            std::cout << "L2 HIT\n";
        } else {
            std::cout << "L2 MISS (Memory Access)\n";
        }
    }

    std::cout << "Physical address: 0x"
              << std::hex << paddr << std::dec << "\n";
}
else if (cmd == "cache_stats") {
    if (L1 && L2) {
        std::cout << "L1 Cache Stats:\n";
        L1->stats();
        std::cout << "L2 Cache Stats:\n";
        L2->stats();
    } else {
        std::cout << "Cache not initialized\n";
    }
}
else if (cmd == "init_vm") {
    size_t vsize, psize, pagesz;
    ss >> vsize >> psize >> pagesz;
    delete vm;
    vm = new VirtualMemory(vsize, psize, pagesz);
    std::cout << "Virtual memory initialized\n";
}
else if (cmd == "vm_access") {
    size_t vaddr;
    ss >> std::hex >> vaddr >> std::dec;

    if (!vm) {
        std::cout << "Virtual memory not initialized\n";
        continue;
    }

    size_t paddr;
    bool page_hit = vm->access(vaddr, paddr);

    if (page_hit)
        std::cout << "PAGE HIT\n";
    else
        std::cout << "PAGE FAULT\n";

    /* ---------- CACHE PIPELINE ---------- */
    if (!L1 || !L2) {
        std::cout << "Cache not initialized\n";
    } else {
        if (L1->access(paddr)) {
            std::cout << "L1 HIT\n";
        } else {
            std::cout << "L1 MISS\n";
            if (L2->access(paddr)) {
                std::cout << "L2 HIT\n";
            } else {
                std::cout << "L2 MISS (Memory Access)\n";
            }
        }
    }

    std::cout << "Physical address: 0x"
              << std::hex << paddr << std::dec << "\n";
}
else if (cmd == "vm_stats") {
    if (vm)
        vm->stats();
    else
        std::cout << "Virtual memory not initialized\n";
}


        /* ---------------- INIT ---------------- */
        else if (cmd == "init") {
            std::string mem;
            size_t size;
            ss >> mem >> size;

            if (buddy_mode) {
                buddy.init(size);
            } else {
                alloc.init(size);
            }
        }

        /* ---------------- SET ALLOCATOR ---------------- */
        else if (cmd == "set") {
            std::string a, p;
            ss >> a >> p;

            if (p == "buddy") {
                buddy_mode = true;
                std::cout << "Buddy allocator enabled\n";
            } else {
                buddy_mode = false;
                if (p == "first_fit")
                    alloc.set_policy(AllocPolicy::FIRST_FIT);
                else if (p == "best_fit")
                    alloc.set_policy(AllocPolicy::BEST_FIT);
                else if (p == "worst_fit")
                    alloc.set_policy(AllocPolicy::WORST_FIT);
            }
        }

        /* ---------------- MALLOC ---------------- */
        else if (cmd == "malloc") {
            size_t size;
            ss >> size;

            if (buddy_mode) {
                int addr = buddy.malloc_block(size);
                if (addr == -1)
                    std::cout << "Buddy allocation failed\n";
                else
                    std::cout << "Allocated at address " << addr << "\n";
            } else {
                int id = -1;
                size_t addr = alloc.malloc_block(size, id);
                if (addr == static_cast<size_t>(-1))
                    std::cout << "Allocation failed\n";
                else
                    std::cout << "Allocated block id=" << id
                              << " at address=0x" << std::hex << addr << std::dec << "\n";
            }
        }

        /* ---------------- FREE ---------------- */
        else if (cmd == "free") {
            int x;
            ss >> x;

            if (buddy_mode) {
                if (buddy.free_block(x))
                    std::cout << "Buddy block freed\n";
                else
                    std::cout << "Invalid address\n";
            } else {
                if (alloc.free_block(x))
                    std::cout << "Block freed\n";
                else
                    std::cout << "Invalid block id\n";
            }
        }

        /* ---------------- DUMP ---------------- */
        else if (cmd == "dump") {
            if (buddy_mode)
                buddy.dump();
            else
                alloc.dump();
        }

        /* ---------------- STATS ---------------- */
        else if (cmd == "stats") {
            if (!buddy_mode)
                alloc.stats();
            else
                std::cout << "Stats not tracked for buddy allocator\n";
        }

        else {
            std::cout << "Unknown command\n";
        }

    }
    delete vm;
    delete L1;
    delete L2;
    return 0;
}
