# Transactional memory

关于 Transactional memory的理论知识，参见工程parallel-computing的`Concurrent-computing\Concurrency-control\Transactional-memory`章节。

## cppreference [Transactional memory](https://en.cppreference.com/w/cpp/language/transactional_memory)

Transactional memory is a concurrency synchronization mechanism that combines groups of statements in transactions, that are

- atomic (either all statements occur, or nothing occurs)
- isolated (statements in a transaction may not observe half-written writes made by another transaction, even if they execute in parallel)

### Synchronized blocks

`synchronized compound-statement`

```C++
#include <iostream>
#include <vector>
#include <thread>
int f()
{
    static int i = 0;
    synchronized { // begin synchronized block
        std::cout << i << " -> ";
        ++i;       // each call to f() obtains a unique value of i
        std::cout << i << '\n';
        return i; // end synchronized block
    }
}
int main()
{
    std::vector<std::thread> v(10);
    for(auto& t: v)
        t = std::thread([]{ for(int n = 0; n < 10; ++n) f(); });
    for(auto& t: v)
        t.join();
}

```

> NOTE: 非常类似于Java的语法。

## gcc.gnu [Transactional Memory in GCC](https://gcc.gnu.org/wiki/TransactionalMemory)