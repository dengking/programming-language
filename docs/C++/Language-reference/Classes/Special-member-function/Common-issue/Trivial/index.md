# Trivial



## [Default constructors](https://en.cppreference.com/w/cpp/language/default_constructor) # Trivial default constructor

The default constructor for class `T` is trivial (i.e. performs no action) if all of the following is true:

1、The constructor is not user-provided (i.e., is implicitly-defined or defaulted on its first declaration)

2、`T` has no virtual member functions

3、`T` has no virtual base classes

4、`T` has no non-static members with default initializers.  (since C++11)

5、Every direct base of `T` has a trivial default constructor

6、Every non-static member of class type (or array thereof) has a trivial default constructor

A trivial default constructor is a constructor that performs no action. All data types compatible with the C language (POD types) are trivially default-constructible.

## Copy

### cppreference [Copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor) # [Trivial copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor#Trivial_copy_constructor)

The copy constructor for class `T` is trivial if all of the following are true:

1、it is not user-provided (that is, it is implicitly-defined or defaulted) ;

2、`T` has no virtual member functions;

3、`T` has no virtual base classes;

4、the copy constructor selected for every direct base of `T` is trivial;

5、the copy constructor selected for every non-static class type (or array of class type) member of `T` is trivial;

A trivial copy constructor for a non-union class effectively copies every scalar subobject (including, recursively, subobject of subobjects and so forth) of the argument and performs no other action. However, padding bytes need not be copied, and even the object representations of the copied subobjects need not be the same as long as their values are identical.

[*TriviallyCopyable*](https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable) objects can be copied by copying their object representations manually, e.g. with [std::memmove](https://en.cppreference.com/w/cpp/string/byte/memmove). All data types compatible with the C language (POD types) are trivially copyable.



### cppreference [Copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment) # Trivial copy assignment operator

The copy assignment operator for class `T` is trivial if all of the following is true:

1、it is not user-provided (meaning, it is implicitly-defined or defaulted) ;

2、`T` has no virtual member functions;

3、`T` has no virtual base classes;

4、the copy assignment operator selected for every direct base of `T` is trivial;

5、the copy assignment operator selected for every non-static class type (or array of class type) member of `T` is trivial.

A trivial copy assignment operator makes a copy of the object representation as if by [std::memmove](https://en.cppreference.com/w/cpp/string/byte/memmove). All data types compatible with the C language (POD types) are trivially copy-assignable.

## Move

### cppreference [Move constructors](https://en.cppreference.com/w/cpp/language/move_constructor) # [Trivial move constructor](https://en.cppreference.com/w/cpp/language/move_constructor#Trivial_move_constructor)

The move constructor for class `T` is trivial if all of the following is true:

1、it is not user-provided (meaning, it is **implicitly-defined** or **defaulted**);

> NOTE: 
>
> 1、需要对 "**implicitly-defined** or **defaulted**" 所执行的行为有完整认知

2、`T` has no virtual member functions;

3、`T` has no virtual base classes;

4、the move constructor selected for every direct base of `T` is trivial;

5、the move constructor selected for every non-static class type (or array of class type) member of `T` is trivial.

A trivial move constructor is a constructor that performs the same action as the trivial copy constructor, that is, makes a copy of the object representation as if by [std::memmove](https://en.cppreference.com/w/cpp/string/byte/memmove). All data types compatible with the C language (POD types) are trivially movable.



### cppreference [Move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment) # Trivial move assignment operator

The move assignment operator for class `T` is trivial if all of the following is true:

1、It is not user-provided (meaning, it is implicitly-defined or defaulted);

2、`T` has no virtual member functions;

3、`T` has no virtual base classes;

4、the move assignment operator selected for every direct base of `T` is trivial;

5、the move assignment operator selected for every non-static class type (or array of class type) member of `T` is trivial.

A trivial move assignment operator performs the same action as the trivial copy assignment operator, that is, makes a copy of the object representation as if by [std::memmove](https://en.cppreference.com/w/cpp/string/byte/memmove). All data types compatible with the C language (POD types) are trivially move-assignable.



### 陷阱: 导致dangling

#### stackoverflow [c++ type trait to say “trivially movable” - examples of](https://stackoverflow.com/questions/45747932/c-type-trait-to-say-trivially-movable-examples-of)



#### 修正方法

参见 `Relocate` 章节。





## Destructors

### cppreference [Destructors](https://en.cppreference.com/w/cpp/language/destructor) # Trivial destructor

The destructor for class `T` is trivial if all of the following is true:

1、The destructor is not user-provided (meaning, it is either implicitly declared, or explicitly defined as defaulted on its first declaration)

2、The destructor is not virtual (that is, the base class destructor is not virtual)

3、All direct base classes have trivial destructors

4、All non-static data members of class type (or array of class type) have trivial destructors

A trivial destructor is a destructor that performs no action. Objects with trivial destructors don't require a delete-expression and may be disposed of by simply deallocating their storage. All data types compatible with the C language (POD types) are trivially destructible.