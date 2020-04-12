# Type system 

## type qualifier

c family language的type system比较类似，都使用[Type qualifier](https://en.wikipedia.org/wiki/Type_qualifier)

## effective type 

c中有[effective type](https://en.cppreference.com/w/c/language/object)的概念，而`c++`中则没有

`c++`中并没有effective type的概念；其实主要原因在于c++中使用了`new`，而c中则是`malloc`，显然这是`c++`在type safety上的一些改善，这一点在[Type safety](https://en.wikipedia.org/wiki/Type_safety#C++)中有解释；

