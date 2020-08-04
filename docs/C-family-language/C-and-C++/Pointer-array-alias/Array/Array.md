# Array



## n-dimensional array



[n-dimensional c++ array. How`s that possible?](https://stackoverflow.com/questions/11583747/n-dimensional-c-array-hows-that-possible)

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



## [Get length of array in C and C++](https://iq.opengenus.org/length-of-array-in-c/)



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

