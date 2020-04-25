# Example of undefined behavior

## Attempting to modify a string literal

cppreference c++ [string literal](https://en.cppreference.com/w/cpp/language/string_literal) 和 cppreference c [string literals](https://en.cppreference.com/w/c/language/string_literal)中，都对此进行了说明，cppreference c [string literals](https://en.cppreference.com/w/c/language/string_literal)对此进行了解释：

> String literals are **not modifiable** (and in fact may be placed in read-only memory such as `.rodata`).

`.rodata`中`ro`的含义是read only。



## Overflow and underflow





## Update one variable more than once "in a sequence point"

在https://stackoverflow.com/a/14041847中给出了例子：

```c++
#include <iostream>
#define SQUARE(x) ((x) * (x))

int main()
{
int x = 2;
int y = SQUARE(x++);
std::cout << x << '\n'
          << y << '\n';

}
```

> It is undefined behaviour to update one variable more than once "in a sequence point". A sequence point is not exactly the same as a statement, but for most intents and purposes, that's what we should consider it as. So doing `x++ * x++` will update `x` twice, which is undefined and will probably lead to different values on different systems, and different outcome value in `x` as well.