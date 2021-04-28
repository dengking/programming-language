# Arena 

## stackoverflow [What is the meaning of the term arena in relation to memory?](https://stackoverflow.com/questions/12825148/what-is-the-meaning-of-the-term-arena-in-relation-to-memory)



### [A](https://stackoverflow.com/a/12825221)

An arena is just a large, contiguous piece of memory that you allocate once and then use to manage memory manually by handing out parts of that memory. 

> NOTE: 
>
> 1、"avoid-system-call-new-malloc-optimization"

For example:

```c++
char * arena = malloc(HUGE_NUMBER);

unsigned int current = 0;

void * my_malloc(size_t n) { current += n; return arena + current - n; }
```

The point is that you get full control over how the memory allocation works. The only thing outside your control is the single library call for the initial allocation.

One popular use case is where each arena is only used to allocate memory blocks of one single, fixed size. In that case, you can write very efficient reclamation algorithms. Another use case is to have one arena per "task", and when you're done with the task, you can free the entire arena in one go and don't need to worry about tracking individual deallocations.

Each of those techniques is very specialized and generally only comes in handy if you know exactly what you're doing and why the normal library allocation is not good enough. Note that a good memory allocator will already do lots of magic itself, and you need a decent amount of evidence that that's not good enough before you start handling memory yourself.