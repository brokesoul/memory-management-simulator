# Memory Management Simulator

## Overview

This project is a **Memory Management Simulator** that models how an operating system manages memory internally.
It simulates:

* Dynamic memory allocation (`malloc` / `free`)
* Multiple allocation strategies (First Fit, Best Fit, Worst Fit)
* Buddy memory allocation
* Virtual memory using paging and page faults
* A multilevel CPU cache hierarchy (L1 and L2)
* FIFO replacement policies
* Correct integration between virtual memory and cache

The simulator is **command-driven**, modular, and focused on **correct behavior**, not real hardware interaction.

---

## Features Implemented

### 1. Physical Memory Simulation

* Memory is modeled as a **contiguous block of bytes**.
* Each block tracks:

  * Start address
  * Size
  * Free / Used state
  * Block ID
* Memory layout can be viewed using `dump`.

---

### 2. Dynamic Memory Allocation

Supports basic heap-style allocation:

* `malloc <size>` allocates memory
* `free <id>` frees memory
* Adjacent free blocks are **automatically coalesced**

---

### 3. Allocation Strategies

The allocator supports three strategies (switchable at runtime):

* **First Fit** – first free block that fits
* **Best Fit** – smallest free block that fits
* **Worst Fit** – largest free block

This allows comparison of fragmentation behavior across strategies.

---

### 4. Fragmentation & Statistics

The simulator tracks:

* Used memory
* Free memory
* Internal fragmentation
* External fragmentation

Statistics are displayed using `stats`.

---

### 5. Buddy Allocator

An optional **Buddy Memory Allocation** system is implemented.

Key characteristics:

* Memory sizes are powers of two
* Allocation sizes are rounded up
* Blocks have unique buddies
* Free blocks merge recursively using XOR-based buddy detection

This allocator is fast and similar to what real OS kernels use.

---

### 6. Multilevel CPU Cache Simulation

The simulator models a **two-level cache hierarchy**:

* **L1 Cache** (small, fast)
* **L2 Cache** (larger, slower)

Each cache level has:

* Configurable cache size
* Configurable block size
* Configurable associativity

  * Direct-mapped (associativity = 1)
  * Set-associative (associativity > 1)

---

### 7. Cache Replacement Policy

* **FIFO (First-Come, First-Served)** replacement
* Implemented per cache set
* Oldest cache line in a set is evicted first

---

### 8. Cache Access Simulation

Two access paths are supported:

#### Physical Access

```
access <physical_address>
```

Flow:

```
Physical Address → L1 → L2 → Memory
```

#### Virtual Access

```
vm_access <virtual_address>
```

Flow:

```
Virtual Address
→ Page Table
→ Physical Address
→ L1 Cache
→ L2 Cache
→ Memory
```

Cache always uses **physical addresses**, which matches real systems.

---

### 9. Virtual Memory (Paging)

The simulator supports paged virtual memory:

* Virtual memory divided into pages
* Physical memory divided into frames
* Page table with valid bit
* Page hits and page faults tracked
* FIFO page replacement when frames are full

Statistics are shown using `vm_stats`.

---

### 10. Integrated Memory Pipeline

The simulator enforces the correct architectural order:

```
Virtual Address
→ Page Table
→ Physical Address
→ L1 Cache
→ L2 Cache
→ Main Memory (symbolic)
```

This is the most important correctness feature of the project.

---

## Project Structure

```
memory-simulator/
├── src/
│   ├── allocator/
│   ├── buddy/
│   ├── cache/
│   ├── virtual_memory/
│   └── main.cpp
├── include/
│   ├── Allocator.h
│   ├── BuddyAllocator.h
│   ├── Cache.h
│   ├── VirtualMemory.h
│   ├── Block.h
│   └── Stats.h
├── tests/
│   ├── test_allocation_strategies.txt
│   ├── test_fragmentation.txt
│   ├── test_buddy_allocator.txt
│   ├── test_multilevel_cache.txt
│   ├── test_virtual_memory.txt
│   └── test_full_pipeline.txt
├── docs/
│   └── design.md
├── Makefile
└── README.md
```

---

## Build & Run

### Compile

```bash
make
```

### Clean build

```bash
make clean
```

### Run

```bash
./memsim
```

---

## Example Commands

### Initialize Memory

```txt
init memory 1024
```

### Set Allocation Strategy

```txt
set allocator first_fit
set allocator best_fit
set allocator worst_fit
set allocator buddy
```

### Allocate / Free

```txt
malloc 100
free 1
```

### View Memory

```txt
dump
stats
```

---

### Initialize Cache

```txt
init_cache 32 16 1   128 16 2
```

### Cache Access

```txt
access 0x100
cache_stats
```

---

### Initialize Virtual Memory

```txt
init_vm 4096 1024 256
```

### Virtual Address Access

```txt
vm_access 0x000
vm_stats
```

---

## Testing

The `tests/` directory contains:

* Input workloads simulating memory allocation patterns
* Cache access workloads
* Virtual address access workloads
* Expected behavior documented as comments
* Optional script to run all tests automatically


---

## Limitations & Simplifications

* Single-process simulation
* No TLB
* No dirty bits or write-back policies
* No real timing simulation
* No instruction cache
* Main memory access is symbolic

These choices keep the simulator focused and understandable.

---

## Summary

This project demonstrates how an operating system:
* Allocates and frees memory
* Manages fragmentation
* Handles virtual memory and page faults
* Uses multilevel CPU caches to speed up memory access

## Demo & Screenshots

### Demo Video
Link: [https://drive.google.com/video_link](https://drive.google.com/file/d/17Gw0cebYqsMpvdI_aOcQolAJ1ESngDiO/view?usp=sharing)

### Screenshots
Screenshots demonstrating:
- Allocation strategies
- Cache hits/misses
- Virtual memory page faults
- Full pipeline execution

(See `screenshots/` folder)



