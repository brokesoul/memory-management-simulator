This folder contains input workloads and expected behaviors for validating
the memory management simulator.

Each test focuses on a specific subsystem:
- Dynamic memory allocation
- Fragmentation behavior
- Cache hierarchy behavior
- Virtual memory paging
- Integrated VM + cache pipeline

Exact numeric outputs may vary depending on configuration, but qualitative
behavior (hits, misses, placement, eviction) must match expectations.
