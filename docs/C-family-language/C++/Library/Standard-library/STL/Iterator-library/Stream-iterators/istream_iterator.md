# `istream_iterator`



## cppreference [std::istream_iterator](https://en.cppreference.com/w/cpp/iterator/istream_iterator)



`std::istream_iterator` is a single-pass input iterator that reads successive objects of type `T` from the [std::basic_istream](https://en.cppreference.com/w/cpp/io/basic_istream) object for which it was constructed, by calling the appropriate `operator>>`. 

> NOTE: `std::istream_iterator`的特性：
>
> - single pass
> - input iterator 



The default-constructed `std::istream_iterator` is known as the *end-of-stream* iterator. When a valid `std::istream_iterator` reaches the end of the underlying stream, it becomes equal to the end-of-stream iterator. Dereferencing or incrementing it further invokes undefined behavior.

> NOTE: 在下面的example中，就用到了“The default-constructed `std::istream_iterator` is known as the *end-of-stream* iterator. ”来标志end-of-stream。

### Notes

When reading characters, `std::istream_iterator` skips **whitespace** by default (unless disabled with [std::noskipws](https://en.cppreference.com/w/cpp/io/manip/skipws) or equivalent), while [std::istreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator) does not. In addition, [std::istreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator) is more efficient, since it avoids the overhead of constructing and destructing the sentry object once per character.



### Example

```c++
#include <iostream>
#include <sstream>
#include <iterator>
#include <numeric>
#include <algorithm>
 
int main()
{
    std::istringstream str("0.1 0.2 0.3 0.4");
    std::partial_sum(std::istream_iterator<double>(str),
                     std::istream_iterator<double>(),
                     std::ostream_iterator<double>(std::cout, " "));
 
    std::istringstream str2("1 3 5 7 8 9 10");
    std::cout << "\nThe first even number is " <<
        *std::find_if(std::istream_iterator<int>(str2),
                      std::istream_iterator<int>(),
                      [](int i){return i%2 == 0;})
        << ".\n";
    //" 9 10" left in the stream
}
```

> NOTE: `std::istream_iterator<double>()`原来标识end-of-stream。



## cppreference [std::istreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator)