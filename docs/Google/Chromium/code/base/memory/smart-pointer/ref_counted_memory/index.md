# [chromium](https://github.com/chromium/chromium)/[base](https://github.com/chromium/chromium/tree/master/base)/[memory](https://github.com/chromium/chromium/tree/master/base/memory)/**[ref_counted_memory.h](https://github.com/chromium/chromium/blob/master/base/memory/ref_counted_memory.h)**

```c++
class BASE_EXPORT RefCountedMemory
    : public RefCountedThreadSafe<RefCountedMemory> 
class BASE_EXPORT RefCountedStaticMemory : public RefCountedMemory
class BASE_EXPORT RefCountedBytes : public RefCountedMemory
class BASE_EXPORT RefCountedString : public RefCountedMemory
class BASE_EXPORT RefCountedString16 : public base::RefCountedMemory 
class BASE_EXPORT RefCountedSharedMemoryMapping : public RefCountedMemory
```

