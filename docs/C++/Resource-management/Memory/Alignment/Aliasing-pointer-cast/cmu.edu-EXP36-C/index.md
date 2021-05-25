# cmu.edu [EXP36-C. Do not cast pointers into more strictly aligned pointer types](https://wiki.sei.cmu.edu/confluence/display/c/EXP36-C.+Do+not+cast+pointers+into+more+strictly+aligned+pointer+types)

Do not convert a pointer value to a pointer type that is more strictly aligned than the referenced type. Different alignments are possible for different types of objects. If the type-checking system is overridden by an explicit cast or the pointer is converted to a void pointer (`void *`) and then to a different type, the alignment of an object may be changed.

> NOTE: 
>
> 一、"If the type-checking system is overridden by an explicit cast or the pointer is converted to a void pointer (`void *`) and then to a different type, the alignment of an object may be changed"
>
> 这是非常能够体现`void *`非static type safety，后面会有例子说明
>
> 二、"aliasing-pointer cast to stricter aligned type-unaligned access-undefined behavior"

If the misaligned pointer is dereferenced, the program may [terminate abnormally](https://wiki.sei.cmu.edu/confluence/display/c/BB.+Definitions#BB.Definitions-abnormaltermination). On some architectures, the cast alone may cause a loss of information even if the value is not dereferenced if the types involved have differing alignment requirements.

> NOTE: 
>
> misaligned pointer是会导致很多问题的





## `void *` 非static type safety

> NOTE: 
>
> 这是非常能够体现`void *`非static type safety、static type safety的写法

### Noncompliant Code Example



```C++
int *loop_function(void *v_pointer) {
  /* ... */
  return v_pointer;
}
  
void func(char *char_ptr) {
  int *int_ptr = loop_function(char_ptr);
 
  /* ... */
}
```

### Compliant Solution

Because the input parameter directly influences the return value, and `loop_function()` returns an object of type `int *`, the formal parameter `v_pointer` is redeclared to accept only an object of type `int *`:

```C++
int *loop_function(int *v_pointer) {
  /* ... */
  return v_pointer;
}
  
void func(int *loop_ptr) {
  int *int_ptr = loop_function(loop_ptr);
 
  /* ... */
}
```



## aliasing、pointer cast to stricter aligned type unaligned access 

> NOTE: 
>
> 使用`memcpy`

### Noncompliant Code Example



```C
#include <string.h>
  
struct foo_header {
  int len;
  /* ... */
};
  
void func(char *data, size_t offset) {
  struct foo_header *tmp;
  struct foo_header header;
 
  tmp = (struct foo_header *)(data + offset);
  memcpy(&header, tmp, sizeof(header));
 
  /* ... */
}
```



### Compliant Solution



```C++
#include <string.h>
  
struct foo_header {
  int len;
  /* ... */
};
   
void func(char *data, size_t offset) {
  struct foo_header header;
  memcpy(&header, data + offset, sizeof(header));
 
  /* ... */
}
```

