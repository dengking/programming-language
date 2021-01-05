# Atomic operations library implementation

## cl.cam.ac.uk [C/C++11 mappings to processors](https://www.cl.cam.ac.uk/~pes20/cpp/cpp0xmappings.html)

For each C/C++11 synchronization operation and architecture, the document aims to provide an instruction sequence that implements the operation on given architecture. This is not the only approach — one could provide a mapping that shows the necessary barriers (or other synchronization mechanism) *between* two program-order adjacent memory operations (either atomic or non-atomic). A good example of this approach is Doug Lea's [cookbook for JVM compiler writers](http://g.oswego.edu/dl/jmm/cookbook.html). While that approach can result in higher-performance mappings, we do not use it here because the resulting tables would be large and we have not investigated correct mappings for all the combinations. The per-operation approach that we take here would benefit from an optimisation pass that removes redundant synchronisation between adjacent operations.

### Architectures

### x86 (including x86-64)

| C/C++11 Operation | x86 implementation                                     |
| ----------------- | ------------------------------------------------------ |
| Load Relaxed:     | MOV (from memory)                                      |
| Load Consume:     | MOV (from memory)                                      |
| Load Acquire:     | MOV (from memory)                                      |
| Load Seq_Cst:     | MOV (from memory)                                      |
| Store Relaxed:    | MOV (into memory)                                      |
| Store Release:    | MOV (into memory)                                      |
| Store Seq Cst:    | `(LOCK) XCHG // alternative: MOV (into memory),MFENCE` |
| Consume Fence:    | `<ignore>`                                             |
| Acquire Fence:    | `<ignore>`                                             |
| Release Fence:    | `<ignore>`                                             |
| Acq_Rel Fence:    | `<ignore>`                                             |
| Seq_Cst Fence:    | MFENCE                                                 |

> NOTE: load对应的read、store对应的是write