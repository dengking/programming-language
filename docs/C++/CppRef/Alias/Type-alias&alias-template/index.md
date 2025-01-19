# Type alias, alias template (since C++11) 

“type alias”即“类型别名”，即给一个类型，起一个新名字。

## cppreference [Type alias, alias template (since C++11)](https://en.cppreference.com/w/cpp/language/type_alias)

**Type alias** is a name that refers to a previously defined type (similar to [`typedef`](https://en.cppreference.com/w/cpp/language/typedef)).

**Alias template** is a name that refers to a family of types.



## C++11 using keyword

c++11引入了`using`关键字。

### cppreference  [using](https://en.cppreference.com/w/cpp/keyword/using)

## Application&example

###  Introduce a member typedef name

在例子中演示了这种写法：

https://en.cppreference.com/w/cpp/language/type_alias

```c++
// type alias can introduce a member typedef name
template<typename T>
struct Container { using value_type = T; };
```

下面是一个demo

```c++
template<class T>
class C {
    using TT = T;
};

int main() {}
```



## See also

internalpointers [The differences between "using" and "typedef" in modern C++](https://www.internalpointers.com/post/differences-between-using-and-typedef-modern-c)

> **A template declaration cannot appear at block scope**: 
>
> Alias templates on the other hand follow the same rules of any other templated thing in C++: they cannot appear inside a block. They are actual template declarations, after all!

stackoverflow [What is the difference between 'typedef' and 'using' in C++11?](https://stackoverflow.com/questions/10747810/what-is-the-difference-between-typedef-and-using-in-c11)

