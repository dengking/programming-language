# Template typedef

https://stackoverflow.com/questions/2795023/c-template-typedef

https://stackoverflow.com/questions/15099826/typedef-with-template-parameter-in-c

https://en.cppreference.com/w/cpp/language/type_alias



##  type alias can introduce a member typedef name

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
class C
{
using TT = T;
};

int main(){}
```

## typedef for partial templates

https://stackoverflow.com/questions/2996914/c-typedef-for-partial-templates

https://probablydance.com/2014/01/16/alias-templates-with-partial-specialization-sfinae-and-everything/

https://www.drdobbs.com/the-new-c-typedef-templates/184403850

https://en.cppreference.com/w/cpp/language/type_alias



## [C++ keywords: using](https://en.cppreference.com/w/cpp/keyword/using)



### declaration of ... shadows template parm ...

http://cplusplus.com/forum/general/33886/

http://forums.codeguru.com/showthread.php?475292-template-error-shadows-template-parm

https://stackoverflow.com/questions/20875033/clang-vs-vcerror-declaration-of-t-shadows-template-parameter



## [What is the difference between 'typedef' and 'using' in C++11?](https://stackoverflow.com/questions/10747810/what-is-the-difference-between-typedef-and-using-in-c11)

https://www.internalpointers.com/post/differences-between-using-and-typedef-modern-c

## friend template

https://stackoverflow.com/questions/8967521/class-template-with-template-class-friend-whats-really-going-on-here