# Multidimensional arrays

## Nesting、containing、recursive-definition

1、Multidimensional arrays是典型的containing关系，我们是可以使用recursion来对它进行计算的；

2、multiple dimension array可以看做是array of array，因此，它是典型的recursive definition



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



### stackoverflow [Why use double indirection? or Why use pointers to pointers?](https://stackoverflow.com/questions/5580761/why-use-double-indirection-or-why-use-pointers-to-pointers)

> NOTE: 
>
> 参见 `Pointer/Multiple-level-pointer` 章节。