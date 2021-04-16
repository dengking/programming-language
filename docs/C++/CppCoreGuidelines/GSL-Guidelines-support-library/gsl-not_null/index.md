# `not_null<T>`



## CppCoreGuidelines `not_null<T>`

### 总结

一、当接口入参中，需要表达not null semantic的时候，可以使用`not_null<T>`。

这一点，其实就是: 

1、CppCoreGuidelines [I.12: Declare a pointer that must not be null as `not_null`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i12-declare-a-pointer-that-must-not-be-null-as-not_null)

2、CppCoreGuidelines [F.23: Use a `not_null<T>` to indicate that "null" is not a valid value](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#f23-use-a-not_nullt-to-indicate-that-null-is-not-a-valid-value)

中表达的含义。

显然，`not_null<T>`需要实现"comparable to nullptr concept"，关于此的实现，参见: 

1、https://github.com/microsoft/GSL/blob/main/include/gsl/pointers





二、一般，在实现move semantic的时候，会将source pointer置为`nullptr`，这就说明对于resource handle member，是不能够使用`not_null<T>`，因为它是可以为null的，显然不是not null。

三、当前，gsl对`not_null`的支持是不够的，它对于如下类型的支持是有限的:

1、`not_null<zstring>`



2、`not_null<span>`

参见: 

github [not_null cannot (and should not) be used with span #399](https://github.com/isocpp/CppCoreGuidelines/issues/399)



四、需要注意的是，`not_null<T>`  的设计是不限于pointer的，所有支持not null semantic的，都可以使用它，比如:

1、pointer

2、smart pointer

3、convertible to `null_ptr_t`



### CppCoreGuidelines [I.12: Declare a pointer that must not be null as `not_null`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i12-declare-a-pointer-that-must-not-be-null-as-not_null)



### CppCoreGuidelines [F.23: Use a `not_null<T>` to indicate that "null" is not a valid value](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#f23-use-a-not_nullt-to-indicate-that-null-is-not-a-valid-value)

#### Reason

Clarity. A function with a `not_null<T>` parameter makes it clear that the caller of the function is responsible for any `nullptr` checks that might be necessary. Similarly, a function with a return value of `not_null<T>` makes it clear that the caller of the function does not need to check for `nullptr`.

> NOTE: 
>
> 1、有些design-by-contract的思想





### CppCoreGuidelines [GSL: Guidelines support library](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#gsl-guidelines-support-library) # [GSL.view: Views](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#gslview-views)

`not_null<T>` // `T` is usually a pointer type (e.g., `not_null<int*>` and `not_null<owner<Foo*>>`) that must not be `nullptr`. `T` can be any type for which `==nullptr` is meaningful.

Use `not_null<zstring>` for C-style strings that cannot be `nullptr`. ??? Do we need a name for `not_null<zstring>`? or is its ugliness a feature?





## bfilipek [How not_null can improve your code?](https://www.bfilipek.com/2017/10/notnull.html)

```cpp
void Foo(int* pInt); // pInt cannot be null
```

I should actually use:

```C++
not_null<int *> pInt
```

