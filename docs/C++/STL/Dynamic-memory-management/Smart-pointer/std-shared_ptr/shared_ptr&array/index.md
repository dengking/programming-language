# `std::shared_ptr` and array

## custom deleter

当使用`std::shared_ptr` to array的时候，需要custom deleter来释放array。

### `array_deleter` function object



参考:

1、stackoverflow [shared_ptr to an array : should it be used?](https://stackoverflow.com/questions/13061979/shared-ptr-to-an-array-should-it-be-used)

测试程序如下:

```C++
#include <memory>
#include <iostream>
/**
 * ns_nim_std::vector<int> 
 */
typedef struct
{
    int *data;
    size_t len;
} CVector_int;

CVector_int CVector_int_ctor(size_t len)
{
    int *data = new int[len];
    CVector_int c_obj;
    c_obj.data = data;
    c_obj.len = len;
    return c_obj;
}

template <typename T>
struct array_deleter
{
    void operator()(T const *p)
    {
        std::cout << "destruct array" << std::endl;
        delete[] p;
    }
};

int main()
{
    {
        CVector_int v = CVector_int_ctor(10);
        std::shared_ptr<int> p(v.data, array_deleter<int>());
    }
}
// g++ test.cpp --std=c++11 -pedantic -Wall -Wextra

```

### `std::default_delete`

参见 cppreference [std::default_delete](https://en.cppreference.com/w/cpp/memory/default_delete)，其中给出了example。

