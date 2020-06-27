# [Advantages of using forward](https://stackoverflow.com/questions/3582001/advantages-of-using-forward)

In perfect forwarding, `std::forward` is used to convert the **named rvalue references** `t1` and `t2` to **unnamed rvalue references**. What is the purpose of doing that? How would that affect the called function `inner` if we leave `t1` & `t2` as lvalues?

```cpp
template <typename T1, typename T2>
void outer(T1&& t1, T2&& t2) 
{
    inner(std::forward<T1>(t1), std::forward<T2>(t2));
}
```

# Perfect forwarding

https://cpppatterns.com/patterns/perfect-forwarding.html#line4

# C++ std::move and std::forward

http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html

# Perfect forwarding and universal references in C++

https://eli.thegreenplace.net/2014/perfect-forwarding-and-universal-references-in-c

# Advantages of using forward

https://stackoverflow.com/questions/3582001/advantages-of-using-forward
