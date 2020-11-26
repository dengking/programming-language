# Array

本章讨论array，重要参考如下两篇文章:

1) stackoverflow [How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c)

2) cppreference [Array declaration](https://en.cppreference.com/w/cpp/language/array)



## Initialize array

https://www.dummies.com/programming/cpp/initializing-an-array-in-c/

https://stackoverflow.com/questions/4057948/initializing-a-member-array-in-constructor-initializer

https://stackoverflow.com/questions/2983819/how-initialize-array-of-classes





## Pointer and array

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

Multidimensional arrays是典型的containing关系，我们是可以使用recursion来对它进行计算的；

### stackoverflow [n-dimensional c++ array. How`s that possible?](https://stackoverflow.com/questions/11583747/n-dimensional-c-array-hows-that-possible)

I find a library analogy very useful to visualizing arrays of multiple dimensions:

- 8-dimensional array is a library
- 7-dimensional array is a floor in a library
- 6-dimensional array is a room on a floor in a library
- 5-dimensional array is a bookcase in a room on a floor in a library
- 4-dimensional array is a shelf in a bookcase in a room on a floor in a library
- 3-dimensional array is a book on a shelf in a bookcase in a room on a floor in a library
- 2-dimensional array is a page in a book on a shelf in a bookcase in a room on a floor in a library
- 1-dimensional array is a line on a page in a book on a shelf in a bookcase in a room on a floor in a library
- 0-dimensional array is a character in a line on a page in a book on a shelf in a bookcase in a room on a floor in a library



## Get length of array in C and C++

### Length in term of element in the array

关于此，在“5.3 Pitfall: Using the C idiom to get number of elements.”中进行了详细说明；

### Length in term of byte

当我们使用c api来操作array的时候，需要使用length in term of byte，主要使用`sizeof`。



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





## TODO

[2 Dimensional Char Array C++](https://stackoverflow.com/questions/27697879/2-dimensional-char-array-c)

[Two-Dimensional Char Array](http://www.cplusplus.com/forum/beginner/53734/)

[valarray](http://www.cplusplus.com/reference/valarray/valarray/)

[Promoting a raw pointer to valarray](https://stackoverflow.com/questions/15077173/promoting-a-raw-pointer-to-valarray)

