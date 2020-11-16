# `std::hash`

在思考使用`const char*`来作为`std::unordered_map`的时候，遇到了hash的问题，然而在cppreference [std::hash](https://en.cppreference.com/w/cpp/utility/hash) 中有这样的描述:

> There is no specialization for C strings. std::hash<const char*> produces a hash of the value of the pointer (the memory address), it does not examine the contents of any character array.

因此，我只能怪customize hash for `const char*`。



## `std::hash` in C++ standard library的概述

在 stackoverflow [std::hash value on char* value and not on memory address?](https://stackoverflow.com/questions/34597260/stdhash-value-on-char-value-and-not-on-memory-address) 的[回答](https://stackoverflow.com/a/34597785)中的一段话较好地概述了`std::hash` in C++ standard library: 

> Sadly enough, the current C++ standard library doesn't provide **general purpose hash algorithms** disentangled(拜托) from **object-specific hash solutions**. (But there is [some hope](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) this could change in the future.)

显然，这段话告诉了我们: 目前C++ standard library的`std::hash`是object-specific hash function，并没有提供general purpose hash algorithm。

### Change in the future: open-std [Types Don't Know](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) 

> NOTE: 在 stackoverflow [std::hash value on char* value and not on memory address?](https://stackoverflow.com/questions/34597260/stdhash-value-on-char-value-and-not-on-memory-address) 的[回答](https://stackoverflow.com/a/34597785)中"But there is [some hope](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) this could change in the future" [some hope](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) 所链接的是 open-std [Types Don't Know](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3980.html) 提案，本文就是基于这篇文章:

This paper proposes a new hashing infrastructure that completely decouples hashing algorithms from individual types that need to be hashed. This decoupling divides the hashing computation among 3 different programmers who need not coordinate with each other.



