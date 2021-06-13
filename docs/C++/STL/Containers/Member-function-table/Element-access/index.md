# cppreference [Containers library](https://en.cppreference.com/w/cpp/container) # Member function table # [Element access](https://en.cppreference.com/w/cpp/container#Member_function_table)

> NOTE: 
>
> 1、优先使用 `at`，为了保证bound safety-bounds checking 
>
> 2、`gsl::at`

## `at` 



Returns a reference to the element at specified location `pos`, with bounds checking.

If `pos` is not within the range of the container, an exception of type [std::out_of_range](https://en.cppreference.com/w/cpp/error/out_of_range) is thrown.

## `operator[]`

Returns a reference to the element at specified location `pos`. No bounds checking is performed.



## `data`



## `front` 和 `back`