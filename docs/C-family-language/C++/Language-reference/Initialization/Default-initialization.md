# Default initialization



## cppreference [Default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)



### Syntax



### Explanation

> NOTE: 从non-class type 和 class type来进行区分

#### Situation

Default initialization is performed in three situations:

| situation                                                    | 注解                                              |
| ------------------------------------------------------------ | ------------------------------------------------- |
| a variable with <br>- automatic, <br>- static <br/>- thread-local [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration) <br>is declared with no **initializer**; |                                                   |
| an object with **dynamic storage duration** <br>- is created by a [new-expression](https://en.cppreference.com/w/cpp/language/new) with no initializer or<br>- is created by a new-expression with the initializer consisting of an empty pair of parentheses (until C++03); |                                                   |
| a base class or a **non-static data member** is not mentioned in a constructor [initializer list](https://en.cppreference.com/w/cpp/language/initializer_list) and that constructor is called. | OOP的情形，重要描述的是**non-static data member** |

#### Effect

The effects of **default initialization** are:

| type                               | effect                                            |
| ---------------------------------- | ------------------------------------------------- |
| a non-POD (until C++11) class type | default constructor                               |
| array type                         | every element of the array is default-initialized |
| otherwise                          | nothing is done                                   |

> NOTE: 原文中的解释其实是非常难以理解的，在下面文章中有非常好的解释:
>
> akrzemi1 [Value-initialization with C++](https://akrzemi1.wordpress.com/2013/09/10/value-initialization-with-c/)，收录在`C++\Language-reference\Initialization\Value-initialization.md`中了。
>
> 在原文的NOTE段，对上面的内容进行了比较直观易懂的解释:

### Notes

Default initialization of **non-class variable**s with **automatic** and **dynamic** storage duration produces objects with indeterminate values (static and thread-local objects get [zero initialized](https://en.cppreference.com/w/cpp/language/zero_initialization))

If `T` is a const-qualified type, it must be a class type with a **user-provided default constructor**.

> NOTE: 也就是built-in type，是不允许default-initialized的。

Reference cannot be default-initialized.