

# 5.4 Other Practicalities

> NOTE: 
>
> 在script language中，可能并没有"by value" semantic，那如何来进行映射呢？原文给出了解答。
>
> 这一段内容是非常好的，它对相关问题的总结是非常值得借鉴的，在进行wrapper编写的时候，是常常会碰到类似的问题的。

## 5.4.1 Passing structures by value

```C++
double dot_product(Vector a, Vector b);
```



```c++
double wrap_dot_product(Vector *a, Vector *b) {
    Vector x = *a;
    Vector y = *b;
    return dot_product(x, y);
}
```



## 5.4.2 Return by value

```C
Vector cross_product(Vector v1, Vector v2);
```



```C
Vector *wrap_cross_product(Vector *v1, Vector *v2) {
    Vector x = *v1;
    Vector y = *v2;
    Vector *result;
    result = (Vector *) malloc(sizeof(Vector));
    *(result) = cross(x, y);
    return result;
}
```



In both cases, SWIG allocates a new object and returns a reference to it. It is up to the user to delete the returned object when it is no longer in use. Clearly, this will leak memory if you are unaware of the implicit memory allocation and don't take steps to free the result. That said, it should be noted that some language modules can now automatically track newly created objects and reclaim memory for you. Consult the documentation for each language module for more details.

It should also be noted that the handling of pass/return by value in C++ has some special cases. For example, the above code fragments don't work correctly if Vector doesn't define a default constructor. The section on SWIG and C++ has more information about this case.



## 5.4.3 Linking to structure variables

> NOTE: 
>
> 这里透露了swig的实现细节

## 5.4.4 Linking to `char *`

When a global variable of type `char *` appears, SWIG uses `malloc()` or `new` to allocate memory for the new value.

> NOTE: 
>
> deep copy to avoid dangling reference



## 5.4.5 Arrays

Arrays are fully supported by SWIG, but they are always handled as pointers instead of mapping them to a special array object or list in the target language.

> NOTE: 
>
> 依然是遵循 **Everything else is a pointer**

Array variables are supported, but are read-only by default. For example:

```C
int a[100][200];
```

In this case, reading the variable '`a`' returns a pointer of type `int (*)[200]` that points to the first element of the array `&a[0][0]`. Trying to modify '`a`' results in an error. This is because SWIG does not know how to copy data from the target language into the array. To work around this limitation, you may want to write a few simple assist functions like this:



## 5.4.7 Renaming and ignoring declarations 



## 5.4.9 Pointers to functions and callbacks

Although SWIG does not normally allow **callback functions** to be written in the target language, this can be accomplished with the use of typemaps and other advanced SWIG features. See the Typemaps chapter for more about typemaps and individual target language chapters for more on callbacks. The 'director' feature can be used to make callbacks from C/C++ into the target language, see Callbacks to the target language.

