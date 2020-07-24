# [C++11 rvalues and move semantics confusion (return statement)](https://stackoverflow.com/questions/4986673/c11-rvalues-and-move-semantics-confusion-return-statement)

I'm trying to understand **rvalue references** and **move semantics** of C++11.

What is the difference between these examples, and which of them is going to do no vector copy?

## First example

```cpp
std::vector<int> return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return tmp;
}

std::vector<int> &&rval_ref = return_vector();
```

## Second example

```cpp
std::vector<int>&& return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return std::move(tmp);
}

std::vector<int> &&rval_ref = return_vector();
```

## Third example

```cpp
std::vector<int> return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return std::move(tmp);
}

std::vector<int> &&rval_ref = return_vector();
```

***comments*** : 

Please do not return local variables by reference, ever. An rvalue reference is still a reference.



## [A](https://stackoverflow.com/a/4986802)

### First example

```cpp
std::vector<int> return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return tmp;
}

std::vector<int> &&rval_ref = return_vector();
```

The first example returns a temporary which is caught by `rval_ref`. That **temporary** will have its life extended beyond the `rval_ref` definition and you can use it as if you had caught it **by value**. This is very similar to the following:

```cpp
const std::vector<int>& rval_ref = return_vector();
```

except that in my rewrite you obviously can't use `rval_ref` in a non-const manner.

***SUMMARY*** : 这段总结非常好，在# [Understanding the warning: binding r-value to l-value reference](https://stackoverflow.com/questions/34240794/understanding-the-warning-binding-r-value-to-l-value-reference) 也对这个问题进行了讨论，并且这篇文章讨论地非常透彻，有收录这篇文章。

### Second example

```cpp
std::vector<int>&& return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return std::move(tmp);
}

std::vector<int> &&rval_ref = return_vector();
```

In the second example you have created a run time error. `rval_ref` now holds a reference to the destructed `tmp` inside the function. With any luck, this code would immediately crash.

### Third example

```cpp
std::vector<int> return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return std::move(tmp);
}

std::vector<int> &&rval_ref = return_vector();
```

Your third example is roughly equivalent to your first. The `std::move` on `tmp` is unnecessary and can actually be a performance pessimization as it will inhibit return value optimization.

The best way to code what you're doing is:

### Best practice

```cpp
std::vector<int> return_vector(void)
{
    std::vector<int> tmp {1,2,3,4,5};
    return tmp;
}

std::vector<int> rval_ref = return_vector();
```

I.e. just as you would in C++03. `tmp` is implicitly treated as an rvalue in the return statement. It will either be returned via **return-value-optimization** (no copy, no move), or if the compiler decides it can not perform RVO, then it [will use vector's move constructor to do the return](https://stackoverflow.com/questions/11088023/is-an-object-guaranteed-to-be-moved-when-it-is-returned). Only if RVO is not performed, and if the returned type did not have a **move constructor** would the **copy constructor** be used for the return.



# [Which constructor will trigger the move semantics?](https://stackoverflow.com/questions/47490610/which-constructor-will-trigger-the-move-semantics)

TODO