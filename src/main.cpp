#include <iostream>
#include <sstream>

#include "Allocator.h"
#include "BuddyAllocator.h"
#include "Cache.h"
#include "VirtualMemory.h"

int main() {
    Allocator alloc;
    BuddyAllocator buddy;

    // Cache hierarchy
    Cache L1(128, 16, 2);   // L1: 128B, 16B block, 2-way
    Cache L2(256, 32, 4);   // L2: 256B, 32B block, 4-way

    // Virtual memory
    VirtualMemory vm(256, 8); // page size 256B, 8 frames

    std::string line;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        std::stringstream ss(line);
        std::string cmd;
        ss >> cmd;

        if (cmd == "init") {
            std::string what;
            size_t size;
            ss >> what >> size;
            alloc.init(size);
            buddy.init(size);
        }

        else if (cmd == "set") {
            std::string what, pol;
            ss >> what >> pol;
            if (pol == "first_fit") alloc.set_policy(AllocPolicy::FIRST_FIT);
            else if (pol == "best_fit") alloc.set_policy(AllocPolicy::BEST_FIT);
            else if (pol == "worst_fit") alloc.set_policy(AllocPolicy::WORST_FIT);
        }

else if (cmd == "malloc") {
    size_t sz;
    ss >> sz;

    int id;
    size_t addr = alloc.malloc_block(sz, id);

    if (id == -1)
        std::cout << "Allocation failed\n";
    else
        std::cout << "Allocated block id=" << id
                  << " at address=0x"
                  << std::hex << addr << std::dec << "\n";
}

        else if (cmd == "free") {
            int id;
            ss >> id;
            if (alloc.free_block(id))
                std::cout << "Block freed\n";
            else
                std::cout << "Invalid block id\n";
        }

        else if (cmd == "access") {
            size_t vaddr;
            ss >> std::hex >> vaddr;

            size_t paddr = vm.access(vaddr);

            bool hit1 = L1.access(paddr);
            if (!hit1) L2.access(paddr);

            std::cout << "Access VA=0x" << std::hex << vaddr
                      << " → PA=0x" << paddr << std::dec << "\n";
        }

        else if (cmd == "dump") {
            alloc.dump();
        }

        else if (cmd == "stats") {
            std::cout << "\n--- Allocator ---\n";
            alloc.stats();

            std::cout << "\n--- Virtual Memory ---\n";
            vm.stats();

            std::cout << "\n--- L1 Cache ---\n";
            L1.stats();

            std::cout << "\n--- L2 Cache ---\n";
            L2.stats();
        }

        else if (cmd == "buddy_dump") {
            buddy.dump();
        }

        else if (cmd == "exit") {
            break;
        }
    }
    return 0;
}

