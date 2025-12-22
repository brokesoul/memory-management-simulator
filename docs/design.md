# Memory Management Simulator — Design Document

## Assumptions
- Memory unit: bytes
- Single-process simulation
- Fixed page size
- FIFO replacement for cache and paging

## Physical Memory
- Linked-list based block allocator
- First Fit, Best Fit, Worst Fit strategies
- Explicit block splitting and coalescing

## Buddy Allocation
- Power-of-two memory sizes
- XOR-based buddy computation
- Recursive split and merge

## Cache Hierarchy
- Set-associative cache
- FIFO replacement
- Physical-address indexed

## Virtual Memory
- Page table with valid bit
- FIFO page replacement
- Separate frame tracking

## Address Translation Flow
Virtual Address → Page Table → Physical Address → Cache → Memory

## Limitations
- No TLB
- No dirty bits
- No multi-process support
