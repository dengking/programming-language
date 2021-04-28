# Custom virtual table

## 总的思想

这种实现思路的总体思想是: function pointer for behavior and `void` pointer for data。

这是在 stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) # [A](https://stackoverflow.com/a/6044720) 中总结的，后来看了很多custom virtual table的实现，都符合这个讨论。

Function pointer的典型的代表就是:

1、`std::function`

2、`boost::function`

## stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) # [A](https://stackoverflow.com/a/6044720) 

The "different" methods simply differ in the way they add semantic sugar. Virtual functions, e.g., are just semantic sugar for

```cpp
struct Class {
    struct vtable {
        void (*dtor)(Class*);
        void (*func)(Class*,double);
    } * vtbl
};
```

iow: function pointers.

## Example code 

在下面文章中给出了example code:

1、sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

2、stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques) 