# Memory Management Simulator – Design

## Assumptions
- Byte-addressable memory
- Single process
- Fixed page size
- FIFO cache & paging replacement

## Memory Allocation
- First Fit / Best Fit / Worst Fit
- Explicit free list
- Immediate coalescing

## Buddy Allocator
- Power-of-two blocks
- XOR buddy computation
- Recursive split & merge

## Cache Hierarchy
- L1 + L2
- Set-associative
- FIFO replacement

## Virtual Memory
- Page table with valid bit
- FIFO page replacement
- Address translation before cache

## Address Flow
Virtual → Page Table → Physical → Cache → Memory

## Limitations
- No TLB
- No dirty bits
- No write-back cache
