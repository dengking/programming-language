# Linkage

## const and linkage

在头文件`BussinessTypes.h`中，如下定义

```
namespace BussinessTypes
{
    
    int StockOpt = 1;
}

```

这种写法是非常不好的，当多个source file包含`BussinessTypes.h`，则会出现：multiple definition of `StockOpt`。

这让我想起来在头文件中声明的`const`常量不会出现这种问题，则将上述修改为

```
namespace BussinessTypes
{
    
    const int StockOpt = 1;
}
```

可以避免上述问题


这让我想起了`const`的linkage，显然它是internal linkage的，下面是参考内容：

- [Why does const imply internal linkage in C++, when it doesn't in C?](https://stackoverflow.com/questions/998425/why-does-const-imply-internal-linkage-in-c-when-it-doesnt-in-c)

  

### constexpr

这是c++11中引入的，那么它是否和`const`一样，是internal linkage呢？

参考：

- [use of constexpr in header file](https://stackoverflow.com/questions/50488831/use-of-constexpr-in-header-file)
- [Quick Q: use of constexpr in header file](https://isocpp.org/blog/2018/05/quick-q-use-of-constexpr-in-header-file)



## linkage 

microsoft [Translation units and linkage](https://docs.microsoft.com/en-us/cpp/cpp/program-and-linkage-cpp?view=vs-2019)

### extern

https://stackoverflow.com/questions/1041866/what-is-the-effect-of-extern-c-in-c?rq=1



## linkage、ORD and header only class

这是接着昨天的关于`const and linkage`而来的，昨天讨论了`const` variable的linkage，显然在一个头文件中的定义的`const` variable，是可以被多态其他的translation unit所引用的，这让我想到了类似的问题：header only class，显然在一个header file中定义了一个class，它可以被多个其他的translation unit所引用，那这是否表明class是internal linkage。

Google：c++ header only class and ODR，下面是比较好的解释：

- [Class definitions in headers and ODR?](https://stackoverflow.com/questions/45900040/class-definitions-in-headers-and-odr)

维基百科[Header-only](https://en.wikipedia.org/wiki/Header-only)
维基百科[One Definition Rule](https://en.wikipedia.org/wiki/One_Definition_Rule)
ORD和linkage之间的关联？linkage是否



## name是否有linkage
ODR与linkage