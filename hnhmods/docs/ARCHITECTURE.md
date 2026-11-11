# Part II: Core Architecture & Memory Foundations

## Memory Architecture

EXAMPLE: Tracking custom heap managers, page allocators, and memory pool strategies.
NOTES:
- allocate using heap thru `MemAlloc_40DEF7`.

## Core Data Structures & Math Libraries

EXAMPLE: Deconstructing SIMD math alignments, custom string tables, and cache-friendly collection structures.
NOTES:
- matrix using old *dxdx* such as `D3DXMatrixMultiply`.
- for struct types in [struct.hpp](../src/struct.hpp)
- for const data in [struct.cpp](../src/struct.cpp)

## Threading & Job Scheduler

EXAMPLE: Multithreading patterns, main thread dispatch loops, thread affinity, and synchronization primitives.

