#ifndef TPF_TRACKNEW_HPP
#define TPF_TRACKNEW_HPP

/*
    From "C++17 - The Complete Guide - By Nicolai M. Josuttis"
	Minor Correction By Thomas Kim
*/

#include <memory_resource>
#include <new>       // for std::align_val_t
#include <cstdio>    // for printf()
#include <cstdlib>   // for malloc() and aligned_alloc()
#ifdef _MSC_VER
#include <malloc.h>  // for _aligned_malloc() and _aligned_free()
#endif

class TrackNew 
{
 private:
  static inline int numMalloc = 0;    // num malloc calls
  static inline size_t sumSize = 0;   // bytes allocated so far
  static inline bool doTrace = false; // tracing enabled
  static inline bool inNew = false;   // don't track output inside new overloads
 public:
  static void reset() 
  {
    // reset new/memory counters
    numMalloc = 0;
    sumSize = 0;
  }

  static void trace(bool b) 
  { 
    // enable/disable tracing
    doTrace = b;
  }

  // implementation of tracked allocation:
  static void* allocate(std::size_t size, std::size_t align, const char* call)
  {
    // track and trace the allocation:
    ++numMalloc;
    sumSize += size;
    void* p;

    if (align == 0) 
    {
      p = std::malloc(size);
    }
    else 
    {
      #ifdef WIN32
        p = _aligned_malloc(size, align);
      #else
        p = std::aligned_alloc(size, align);
      #endif
    }
    
    if (doTrace) 
    {
      // DON'T use std::cout here because it might allocate memory
      // while we are allocating memory (core dump at best)
      printf("#%d %s ", numMalloc, call);
      printf("(%zu bytes, ", size);
      if (align > 0) 
      {
        printf("%zu-byte aligned) ", align);
      }
      else 
      {
        printf("def-aligned) ");
      }

      printf("=> %p (total: %zu bytes)\n", (void*)p, sumSize);
    }

    return p;
  }

  static void status() 
  { 
    // print current state
    printf("%d allocations for %zu bytes\n", numMalloc, sumSize);
  }
};

[[nodiscard]]
void* operator new (std::size_t size) 
{
  return TrackNew::allocate(size, 0, "::new");
}

[[nodiscard]]
void* operator new (std::size_t size, std::align_val_t align) 
{
  return TrackNew::allocate(size, static_cast<size_t>(align), "::new aligned");
}

[[nodiscard]]
void* operator new[] (std::size_t size) 
{
  return TrackNew::allocate(size, 0, "::new[]");
}

[[nodiscard]]
void* operator new[] (std::size_t size, std::align_val_t align) 
{
  return TrackNew::allocate(size, static_cast<size_t>(align), "::new[] aligned");
}

// ensure deallocations match:
void operator delete (void* p) noexcept 
{
  std::free(p);
}

void operator delete (void* p, std::size_t) noexcept 
{
  ::operator delete(p);
}

void operator delete (void* p, std::align_val_t) noexcept 
{
#ifdef WIN32
  _aligned_free(p);  // Windows API
#else
  std::free(p);      // C++17 API
#endif
}

void operator delete (void* p, std::size_t, std::align_val_t align) noexcept 
{
  ::operator delete(p, align);
}

#endif // TPF_TRACKNEW_HPP