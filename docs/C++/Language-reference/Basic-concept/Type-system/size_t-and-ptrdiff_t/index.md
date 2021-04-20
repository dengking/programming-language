# `size_t` and `ptrdiff_t`

主要参考文章: 

1、pvs-studio [About size_t and ptrdiff_t](https://pvs-studio.com/en/a/0050/)

## 总结如下

一、

|          |             |             |
| -------- | ----------- | ----------- |
| unsigned | `size_t`    | `uintptr_t` |
| signed   | `ptrdiff_t` | `intptr_t`  |
|          |             |             |

1、cppreference [ptrdiff_t](https://en.cppreference.com/w/c/types/ptrdiff_t)

> `ptrdiff_t` acts as the signed counterpart of [size_t](https://en.cppreference.com/w/c/types/size_t): it can store the size of the array of any type and is, on most platforms, synonymous with `intptr_t`).



二、

它们都和pointer有着密切关联: 

`size_t` length等于pointer length

`ptrdiff_t` length等于pointer length

在 pvs-studio [About size_t and ptrdiff_t](https://pvs-studio.com/en/a/0050/) # "Portability of `size_t` and `ptrdiff_t`"段中，有着这样对待总结: 

> The size of `size_t` and `ptrdiff_t` always coincide with the pointer's size. Because of this, it is these types which should be used as indexes for large arrays, for storage of pointers and, pointer arithmetic.



三、`ptrdiff_t` 和 `size_t` 都是与机器相关的数据类型，因此它们都是implementation defined。



四、使用它们能够实现:

1、Portability 

2、Safety of `ptrdiff_t` and `size_t` types in address arithmetic

3、Performance of code using `ptrdiff_t` and `size_t`

省去了data conversion command

## stackoverflow [C++ size_t or ptrdiff_t](https://stackoverflow.com/questions/4377308/c-size-t-or-ptrdiff-t)





## 对比`int`

`int`是fixed length的，是机器不相关的；

