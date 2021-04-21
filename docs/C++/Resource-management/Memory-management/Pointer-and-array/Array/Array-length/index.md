# C array length



## stackoverflow [Calculate Length of Array in C by Using Function](https://stackoverflow.com/questions/4162923/calculate-length-of-array-in-c-by-using-function)	



### [A](https://stackoverflow.com/a/4162948)

You cannot calculate the **size of an array** when all you've got is a **pointer**.

The only way to make this "function-like" is to define a macro:

```c
#define ARRAY_SIZE( array ) ( sizeof( array ) / sizeof( array[0] ) )
```

This comes with all the usual caveats of macros, of course.

> NOTE : 上面这句话的意思是既然上述方法是使用的macro，那么它就无法避免macro的弊端；

**Edit:** (The comments below really belong into the answer...)

1、You **cannot** determine the number of elements *initialized* within an array, unless you initialize all elements to an "invalid" value first and doing the counting of "valid" values manually. If your array has been defined as having 8 elements, for the compiler it has 8 elements, no matter whether you initialized only 5 of them.

2、You **cannot** determine the size of an array within a function to which that array has been passed as parameter. Not directly, not through a macro, not in any way. You can **only** determine the size of an array *in the scope it has been declared in*.

> NOTE : 这段话总结的非常好；

The impossibility of determining the size of the array in a **called function** can be understood once you realize that **`sizeof()` is a compile-time operator**. It might *look* like a run-time function call, but it isn't: The *compiler* determines the size of the operands, and inserts them as constants.

In the scope the array is declared, the compiler has the information that it is actually an array, and how many elements it has.

In a function to which the array is passed, all the compiler sees is a pointer. (Consider that the function might be called with many *different* arrays, and remember that `sizeof()` is a *compile-time operator*.

You can switch to C++ and use `<vector>`. You can define a `struct vector` plus functions handling that, but it's not really comfortable:

```c
#include <stdlib.h>

typedef struct
{
    int *  _data;
    size_t _size;
} int_vector;

int_vector * create_int_vector( size_t size )
{
    int_vector * _vec = malloc( sizeof( int_vector ) );
    if ( _vec != NULL )
    {
        _vec._size = size;
        _vec._data = (int *)malloc( size * sizeof( int ) );
    }
    return _vec;
}

void destroy_int_vector( int_vector * _vec )
{
    free( _vec->_data );
    free( _vec );
}

int main()
{
    int_vector * myVector = create_int_vector( 8 );
    if ( myVector != NULL && myVector->_data != NULL )
    {
        myVector->_data[0] = ...;
        destroy_int_vector( myVector );
    }
    else if ( myVector != NULL )
    {
        free( myVector );
    }
    return 0;
}
```

Bottom line: C arrays are limited. You cannot calculate their length in a sub-function, period. You have to code your way around that limitation, or use a different language (like C++).