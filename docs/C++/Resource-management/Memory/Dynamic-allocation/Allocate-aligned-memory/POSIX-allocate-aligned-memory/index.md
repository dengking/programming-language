# POSIX allocate aligned memory

## stackoverflow [aligned malloc() in GCC?](https://stackoverflow.com/questions/3839922/aligned-malloc-in-gcc) # [A](https://stackoverflow.com/a/3839953)

See the [memalign](http://www.kernel.org/doc/man-pages/online/pages/man3/posix_memalign.3.html) family of functions.



## [posix_memalign(3)](https://man7.org/linux/man-pages/man3/posix_memalign.3.html)

```C++
       #include <stdlib.h>

       int posix_memalign(void **memptr, size_t alignment, size_t size);
       void *aligned_alloc(size_t alignment, size_t size);
       void *valloc(size_t size);

       #include <malloc.h>

       void *memalign(size_t alignment, size_t size);
       void *pvalloc(size_t size);
```

