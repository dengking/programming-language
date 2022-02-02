# `std::allocator`

这是default allocator

## cppreference [std::allocator](https://en.cppreference.com/w/cpp/memory/allocator)

The `std::allocator` class template is the default [`Allocator`](https://pd.codechef.com/docs/cpp/cpp/concept/Allocator.html) used by all standard library containers if no user-specified allocator is provided. 

The default allocator is stateless, that is, all instances of the given allocator are interchangeable, compare equal and can deallocate memory allocated by any other instance of the same allocator type.

> NOTE: 
>
> 一、通过上述内容，我觉得它是在强调`std::allocator`其实是一个简单的abstraction layer

|                                                              |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [(constructor)](https://pd.codechef.com/docs/cpp/cpp/memory/allocator/allocator.html) | creates a new allocator instance (public member function)    |
| [(destructor)](https://pd.codechef.com/docs/cpp/cpp/memory/allocator/~allocator.html) | destructs an allocator instance (public member function)     |
| [address](https://pd.codechef.com/docs/cpp/cpp/memory/allocator/address.html) (deprecated in C++17) | obtains the address of an object, even if operator& is overloaded (public member function) |
| [allocate](https://pd.codechef.com/docs/cpp/cpp/memory/allocator/allocate.html) | allocates uninitialized storage (public member function)     |
| [deallocate](https://pd.codechef.com/docs/cpp/cpp/memory/allocator/deallocate.html) | deallocates storage (public member function)                 |
| [max_size](https://pd.codechef.com/docs/cpp/cpp/memory/allocator/max_size.html) (deprecated in C++17) | returns the largest supported allocation size (public member function) |
| [construct](https://pd.codechef.com/docs/cpp/cpp/memory/allocator/construct.html) (deprecated in C++17) | constructs an object in allocated storage (public member function) |
| [destroy](https://pd.codechef.com/docs/cpp/cpp/memory/allocator/destroy.html) (deprecated in C++17) | destructs an object in allocated storage (public member function) |

> NOTE: 
>
> 对应了object lifetime