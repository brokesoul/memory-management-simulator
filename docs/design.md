# Design and Implementation of a Memory Management Simulator

## 1. Memory Layout and Assumptions

### Memory Model

* The simulator models **physical memory as a contiguous array of bytes**.
* All addresses are **byte-addressable** and displayed in hexadecimal.
* The simulator runs in **user space** and does not manage real system memory.

### Assumptions

* Single-process system (no multi-process isolation).
* Memory size is fixed at initialization time.
* No alignment constraints beyond byte granularity.
* No real hardware interaction; all timing and memory accesses are symbolic.

---

## 2. Allocation Strategy Implementations

The simulator supports **dynamic memory allocation** using a linked-list–based free list.

### Block Representation

Each memory block contains:

* Start address
* Block size
* Allocation status (free / used)
* Block ID (for deallocation)
* Pointers to previous and next blocks

### Supported Strategies

#### First Fit

* Traverses the free list in address order.
* Allocates the **first block large enough** to satisfy the request.
* Fast but may cause early fragmentation.

#### Best Fit

* Scans all free blocks.
* Selects the **smallest block** that can satisfy the request.
* Reduces wasted space but increases fragmentation over time.

#### Worst Fit

* Scans all free blocks.
* Selects the **largest available block**.
* Attempts to reduce small unusable fragments.

### Block Management

* Blocks are **split** on allocation if larger than required.
* Adjacent free blocks are **coalesced on deallocation** to reduce external fragmentation.

---

## 3. Buddy System Design

An optional **Buddy Memory Allocation** scheme is implemented.

### Key Characteristics

* Total memory size is rounded to the **nearest power of two**.
* Allocation sizes are rounded up to powers of two.
* Memory blocks are managed using **free lists indexed by block size (order)**.

### Buddy Calculation

* Buddy addresses are computed using **XOR operations**.
* Recursive splitting occurs during allocation.
* Recursive merging occurs during deallocation when buddies are free.

### Benefits

* Fast allocation and deallocation.
* Simplified coalescing logic.
* Reduced external fragmentation.

---

## 4. Cache Hierarchy and Replacement Policy

### Cache Levels

* **L1 Cache**
* **L2 Cache**
* (L3 cache can be added similarly but is optional)

Each cache level is independently configurable with:

* Cache size
* Block size
* Associativity (direct-mapped or set-associative)

### Cache Structure

* Cache is divided into **sets**, each containing multiple cache lines.
* Each cache line stores:

  * Valid bit
  * Tag

### Replacement Policy

* **FIFO (First-Come, First-Served)** replacement is used.
* Each set maintains a FIFO queue to track insertion order.
* The oldest cache line is evicted on a miss when the set is full.

---

## 5. Virtual Memory Model

The simulator implements **paged virtual memory**.

### Page Table

* Implemented as a simple array indexed by virtual page number.
* Each page table entry stores:

  * Valid bit
  * Physical frame number

### Paging Behavior

* Virtual addresses are divided into:

  * Page number
  * Offset
* On access:

  * Valid page → Page Hit
  * Invalid page → Page Fault

### Page Replacement

* FIFO page replacement is used.
* When physical memory is full, the oldest page is evicted.

---

## 6. Address Translation Flow

The simulator enforces the **correct architectural access order**:

```
Virtual Address
      ↓
Page Table Lookup
      ↓
Physical Address
      ↓
L1 Cache
      ↓
L2 Cache
      ↓
Main Memory (symbolic)
```

### Notes

* Cache accesses always use **physical addresses**.
* Virtual memory translation is fully independent of caching logic.
* Physical memory access is symbolic and used only for correctness modeling.

---

## 7. Limitations and Simplifications

To keep the simulator focused and manageable, the following simplifications were made:

* No Translation Lookaside Buffer (TLB)
* No dirty bits or write-back/write-through policies
* No multi-process support
* No real timing simulation (latency is symbolic)
* No instruction cache (data cache only)

These choices allow clear demonstration of core memory-management concepts without unnecessary complexity.

---

## 8. Summary

This simulator accurately models:

* Dynamic memory allocation strategies
* Buddy memory allocation
* Multilevel CPU cache hierarchy with FIFO replacement
* Virtual memory with paging and page faults
* Correct integration of address translation and caching



## Limitations
- No TLB
- No dirty bits
- No multi-process support
