# `std::shared_ptr` and array

## 测试程序

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



## 参考:

1、stackoverflow [shared_ptr to an array : should it be used?](https://stackoverflow.com/questions/13061979/shared-ptr-to-an-array-should-it-be-used)