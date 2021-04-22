# Array

本章讨论array，重要参考如下两篇文章:

1) stackoverflow [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c)

2) cppreference [Array declaration](https://en.cppreference.com/w/cpp/language/array)



## Pointer and array

### Array to pointer decay

一维array会decay to 一级pointer，显然两者能够进行对应；static array vs dynamic array，其实dynamic array就是一个pointer；

因为array和memory的结构的相同：都是sequence，这就导致了两者的类似；

array和pointer有着相同的arithmetic；

上述两个相似点，导致了pointer和array的很多地方的相似；

Multidimensional array和multiple-level pointer有着对应关系的；可以使用一个multiple-level pointer来访问一个multidimensional array，典型的例子：

1 `char **argv`和`char* argv[]`



Multidimensional array和multiple-level pointer都可以使用containing关系来进行理解；



n-dimensional array、array of pointer、multiple-degree and n-dimensional array

https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c?noredirect=1&lq=1

## Multidimensional arrays

参见 `Multidimensional-array` 章节。





## [With arrays, why is it the case that a[5] == 5[a]?](https://stackoverflow.com/questions/381542/with-arrays-why-is-it-the-case-that-a5-5a)

As Joel points out in [Stack Overflow podcast #34](https://stackoverflow.blog/2008/12/18/podcast-34/), in [C Programming Language](https://rads.stackoverflow.com/amzn/click/com/0131103628) (aka: K & R), there is mention of this property of arrays in C: `a[5] == 5[a]`

Joel says that it's because of pointer arithmetic but I still don't understand. **Why does `a[5] == 5[a]`**?



### [A](https://stackoverflow.com/a/381549)

The C standard defines the `[]` operator as follows:

```
a[b] == *(a + b)
```

Therefore `a[5]` will evaluate to:

```c
*(a + 5)
```

and `5[a]` will evaluate to:

```c
*(5 + a)
```

`a` is a pointer to the first element of the array. `a[5]` is the value that's 5 **elements** further from `a`, which is the same as `*(a + 5)`, and from elementary school math we know those are equal (addition is [commutative](https://en.wikipedia.org/wiki/commutative)).

### [A](https://stackoverflow.com/a/381554)

```c++
#include <iostream>

int main(int argc, char** argv)
{
	std::cout << std::boolalpha << std::endl;
	bool Equal = "ABCD"[2] == 2["ABCD"];
	std::cout << Equal << std::endl;
	Equal = 2["ABCD"] == 'C';
	std::cout << Equal << std::endl;
	Equal = "ABCD"[2] == 'C';
	std::cout << Equal << std::endl;
}

```



## Array class



1) Dynamic array: `std::vector<T>` since C++98 

2) Static array: [`std::array`](http://en.cppreference.com/w/cpp/container/array) since [C++11](https://en.wikipedia.org/wiki/C++11)



## TODO

[2 Dimensional Char Array C++](https://stackoverflow.com/questions/27697879/2-dimensional-char-array-c)

[Two-Dimensional Char Array](http://www.cplusplus.com/forum/beginner/53734/)

[valarray](http://www.cplusplus.com/reference/valarray/valarray/)

[Promoting a raw pointer to valarray](https://stackoverflow.com/questions/15077173/promoting-a-raw-pointer-to-valarray)

