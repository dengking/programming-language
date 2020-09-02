# Trailing return type

这是c++11引入的新特性，首先搞清楚它解决的是什么问题，然后再来学习它的具体的用法。

## cppreference [Function declaration](https://en.cppreference.com/w/cpp/language/function)

**Trailing return type**, useful if the **return type** depends on **argument names**, such as `template <class T, class U> auto add(T t, U u) -> decltype(t + u);` or is complicated, such as in `auto fpif(int)->int(*)(int)`



## Usage

参考：

- stackoverflow [Should the trailing return type syntax style become the default for new C++11 programs? ](https://stackoverflow.com/questions/11215227/should-the-trailing-return-type-syntax-style-become-the-default-for-new-c11-pr)
- ibm [Introduction to the C++11 feature: trailing return types](https://community.ibm.com/community/user/ibmz-and-linuxone/blogs/fang-lu2/2020/03/24/introduction-to-the-c11-feature-trailing-return-types?lang=en)

### Return type depends on argument names

典型的template metaprogramming。

在stackoverflow [Should the trailing return type syntax style become the default for new C++11 programs? [closed]](https://stackoverflow.com/questions/11215227/should-the-trailing-return-type-syntax-style-become-the-default-for-new-c11-pr)的 [回答](https://stackoverflow.com/a/11215414) 中，介绍了这种用法:

> Also, if your return type utilizes a `decltype` that requires that the argument names are in scope, a trailing return type must be used (however, one can usually use `declval<T>` to work around this latter issue).

在 ibm [Introduction to the C++11 feature: trailing return types](https://community.ibm.com/community/user/ibmz-and-linuxone/blogs/fang-lu2/2020/03/24/introduction-to-the-c11-feature-trailing-return-types?lang=en) 中进行了详细介绍。

```c++
#include <iostream>

template<class T>
auto mul(T a, T b) -> decltype(a*b)
{

	return a * b;

}

int main()
{
	std::cout << mul(1, 3);
}

```



#### Idiom: Trailing return type + expression SFINAE

典型的template metaprogramming。

trailing return type：trailing return type + expression SFINAE是一个非常典型的例子。



#### Examples

在stackoverflow [How can I output the value of an enum class in C++11](https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11)的[回答](https://stackoverflow.com/a/11421471)中给出的一个例子：

```c++
template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}


std::cout << as_integer(a) << std::endl;
```



### Lambda return type

在stackoverflow [Should the trailing return type syntax style become the default for new C++11 programs? [closed]](https://stackoverflow.com/questions/11215227/should-the-trailing-return-type-syntax-style-become-the-default-for-new-c11-pr)的 [回答](https://stackoverflow.com/a/11215414) 中，介绍了这种用法:

> There are certain cases where you must use a trailing return type. Most notably, a lambda return type, if specified, must be specified via a trailing return type.



### Improvement of readability and maintainability

本节标题是源自 ibm [Introduction to the C++11 feature: trailing return types](https://community.ibm.com/community/user/ibmz-and-linuxone/blogs/fang-lu2/2020/03/24/introduction-to-the-c11-feature-trailing-return-types?lang=en) 。

#### Make code neat



In his ["Fresh Paint"](http://www.youtube.com/watch?v=ourmFvidEpk&feature=plcp) session at C++Now 2012, Alisdair Meredith pointed out that if you use trailing return types consistently, the names of all of your functions line up neatly:

```cpp
auto foo() -> int;
auto bar() -> really_long_typedef_name;
```

I've used trailing return types everywhere in [CxxReflect](https://github.com/dbremner/cxxreflect/), so if you're looking for an example of how code looks using them consistently, you can take a look there (e.g, [the `type` class](https://github.com/dbremner/cxxreflect/blob/master/cxxreflect/reflection/type.hpp)).



#### 节省代码

在stackoverflow [Should the trailing return type syntax style become the default for new C++11 programs? [closed]](https://stackoverflow.com/questions/11215227/should-the-trailing-return-type-syntax-style-become-the-default-for-new-c11-pr)的 [回答](https://stackoverflow.com/a/11215414) 中，介绍了这种用法:

The trailing return type does have some other minor advantages. For example, consider a non-inline member function definition using the traditional function syntax:

```cpp
struct my_awesome_type
{
    typedef std::vector<int> integer_sequence;

    integer_sequence get_integers() const;
}; 

my_awesome_type::integer_sequence my_awesome_type::get_integers() const
{
    // ...
}
```

Member typedefs are not in scope until after the name of the class appears before `::get_integers`, so we have to repeat the class qualification twice. If we use a trailing return type, we don't need to repeat the name of the type:

```cpp
auto my_awesome_type::get_integers() const -> integer_sequence
{
    // ...
}
```

In this example, it's not such a big deal, but if you have long class names or member functions of class templates that are not defined inline, then it can make a big difference in readability.



在stackoverflow [Should the trailing return type syntax style become the default for new C++11 programs? [closed]](https://stackoverflow.com/questions/11215227/should-the-trailing-return-type-syntax-style-become-the-default-for-new-c11-pr)的 [回答](https://stackoverflow.com/a/11215536) 中，介绍了这种用法:

See this nice article: http://www.cprogramming.com/c++11/c++11-auto-decltype-return-value-after-function.html Very good example when to use this syntax without decltype in game:

```cpp
class Person
{
public:
    enum PersonType { ADULT, CHILD, SENIOR };
    void setPersonType (PersonType person_type);
    PersonType getPersonType ();
private:
    PersonType _person_type;
};

auto Person::getPersonType () -> PersonType
{
    return _person_type;
}
```

And brilliant explanation also stolen from Alex Allain's article "Because the return value goes at the end of the function, instead of before it, you don't need to add the class scope."

Compare to this possible case when one by accident forget about class scope, and, for bigger disaster, another PersonType is defined in global scope:

```cpp
typedef float PersonType; // just for even more trouble
/*missing: Person::*/
PersonType Person::getPersonType ()
{
    return _person_type;
}
```





### Allows to use `this`

In addition to what others said, the **trailing return type** also allows to use `this`, which is not otherwise allowed

> NOTE: 根据`this`的CV来灵活选择`const` 版本的函数。

```cpp
#include <vector>
struct A
{
	std::vector<int> a;

	// OK, works as expected
	auto begin() const -> decltype(a.begin())
	{
		return a.begin();
	}

	// FAIL, does not work: "decltype(a.end())" will be "iterator", but
	// the return statement returns "const_iterator"
	decltype(a.end()) end() const
	{
		return a.end();
	}
};

int main()
{

}
// g++ --std=c++11 test.cpp
```

> NOTE: 编译报错如下:
>
> ```C++
> test.cpp: 在成员函数‘std::vector<int>::iterator A::end() const’中:
> test.cpp:10:48: 错误：could not convert ‘((const A*)this)->A::a.std::vector<_Tp, _Alloc>::end<int, std::allocator<int> >()’ from ‘std::vector<int>::const_iterator {aka __gnu_cxx::__normal_iterator<const int*, std::vector<int> >}’ to ‘std::vector<int>::iterator {aka __gnu_cxx::__normal_iterator<int*, std::vector<int> >}’
>    decltype(a.end()) end() const { return a.end(); }
> ```
>
> 因为函数`const`修饰，所以在函数体中，`this`会被解释为`(const A*)this`， 所以compiler在编译`return a.end();`的时候，其实类似于如下语句:
>
> ```C++
> return (const A*)this)->a.end();
> ```
>
> 所以会选择 `std::vector::end()` 返回值类型为 `const_iterator` 的版本，而 `decltype(a.end())` 的类型是 `iterator`，显然，和返回值的类型是不一致的。

In the second declaration, we used the traditional style. However since `this` is not allowed at that position, the compiler does not implicitly use it. So the `a.end()` uses the statically declared type of `a` to determine what `end` overload of `vector<int>` it is going to call, which ends up being the non-const version.

## TO READ

arne-mertz [Trailing return types everywhere](https://arne-mertz.de/2016/11/trailing-return-types-everywhere/)





