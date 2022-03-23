# Sum

## stackoverflow [How to sum up elements of a C++ vector?](https://stackoverflow.com/questions/3221812/how-to-sum-up-elements-of-a-c-vector)



### [A](https://stackoverflow.com/a/3221813)

Actually there are quite a few methods.

```
int sum_of_elems = 0;
```

#### C++03

1、Classic for loop:

```c++
for(std::vector<int>::iterator it = vector.begin(); it != vector.end(); ++it)
    sum_of_elems += *it;
```

2、Using a standard algorithm:

```c++
#include <numeric>

sum_of_elems = std::accumulate(vector.begin(), vector.end(), 0);
```

**Important Note:** The last argument's type is used not just for the initial value, but for *the type of the result* as well. If you put an int there, it will accumulate ints even if the vector has float. If you are summing floating-point numbers, change `0` to `0.0` or `0.0f` (thanks to nneonneo). See also the C++11 solution below.

#### C++11 and higher

1. b. Automatically keeping track of the vector type even in case of future changes:

    ```C++
    #include <numeric>
    
    sum_of_elems = std::accumulate(vector.begin(), vector.end(),
                                   decltype(vector)::value_type(0));
    ```

2. Using `std::for_each`:

    ```C++
    std::for_each(vector.begin(), vector.end(), [&] (int n) {
        sum_of_elems += n;
    });
    ```

3. Using a range-based for loop (thanks to Roger Pate):

    ```C++
    for (auto& n : vector)
        sum_of_elems += n;
    ```