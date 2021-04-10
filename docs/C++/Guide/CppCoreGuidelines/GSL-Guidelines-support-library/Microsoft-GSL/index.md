# [microsoft](https://github.com/microsoft)/[GSL](https://github.com/microsoft/GSL)

1、使用要求: C++14

看了一些源代码，在[`string_span.h`](https://github.com/microsoft/GSL/blob/main/include/gsl/string_span)中，使用使用C++14 variable template:

```C++
template <typename CharT, std::size_t Extent = dynamic_extent>
using basic_zstring = CharT*;
```



## github [microsoft](https://github.com/microsoft)/[GSL](https://github.com/microsoft/GSL)

## Read the code

实现从最最基础的、被使用最多的开始阅读，即[`assert.h`](https://github.com/microsoft/GSL/blob/main/include/gsl/assert)，其中的`Expects(cond)`、`Ensures(cond)`在整个library中被广泛使用，这是典型的design by contract。



