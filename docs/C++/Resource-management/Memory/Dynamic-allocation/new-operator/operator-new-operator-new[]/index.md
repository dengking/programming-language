# cppreference [operator new, operator new[]](https://en.cppreference.com/w/cpp/memory/new/operator_new)

> NOTE: 
>
> 1、custom allocation operator overload new delete
>
> 2、原文对alignment进行了详细说明

Defined in header [`<new>`](https://en.cppreference.com/w/cpp/header/new)

## Function signature



### replaceable allocation functions



### replaceable non-throwing allocation functions



### non-allocating placement allocation functions





### user-defined placement allocation functions



### class-specific allocation functions



### class-specific placement allocation functions



1)

This function is required to return a pointer suitably **aligned** to point to an object of the requested size.



### Return value

1-4) non-null pointer to suitably **aligned** memory of size at least `size`

5-8) non-null pointer to suitably **aligned** memory of size at least `size`, or null pointer on allocation failure

9-10) `ptr`

11-22) same as (1-4) if the function does not return on allocation failure, otherwise same as (5-8)