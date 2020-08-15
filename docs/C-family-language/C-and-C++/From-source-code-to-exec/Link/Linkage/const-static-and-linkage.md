# const、static and linkage

`const`、`static` 修饰的variable都具备internal linkage。这是因为：

`const`、`static`所修饰的variable都是compile-time的，它们的value在compile-time就已经已知；在一个translation unit中，通过extern声明了一个variable，显然在compile-time是无法获知它的value的，显然，这是矛盾的，所以需要将它们的linkage指定为internal了；

下面是参考内容：

- [Why does const imply internal linkage in C++, when it doesn't in C?](https://stackoverflow.com/questions/998425/why-does-const-imply-internal-linkage-in-c-when-it-doesnt-in-c)

## `const` and linkage

下面是我改写[How do I use arrays in C++?](https://stackoverflow.com/questions/4810664/how-do-i-use-arrays-in-c)的[5.1 Pitfall: Trusting type-unsafe linking.](https://stackoverflow.com/a/7439261)中的例子：



```c++
// [main.cpp]
#include <iostream>
extern const int length;
extern int* numbers;
void print_array();

int main()
{
	print_array();
	using namespace std;
	std::cout << numbers << std::endl;

	for (int i = 0; i < length; ++i)
	{
		cout << (i > 0 ? ", " : "") << (numbers + i * sizeof(int));
	}
	cout << endl;
}

```



```c++
// [numbers.cpp]
#include <iostream>
const int length = 9;
int numbers[length] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

void print_array()
{
	std::cout << numbers << std::endl;
	for (int i = 0; i < length; ++i)
	{
		std::cout << numbers + i << std::endl;
		std::cout << numbers[i] << std::endl;
	}
}

```

编译: `g++ main.cpp numbers.cpp -g -o main`，报错如下：

```c++
/tmp/ccH5XSWb.o：在函数‘main’中：
/home/ust/main.cpp:13：对‘length’未定义的引用
collect2: 错误：ld 返回 1

```

上述问题，让我思考：`const`和`extern`。Google “extern const undefined reference c++”，下面是正解：

### stackoverflow [Why does “extern const int n;” not work as expected?](https://stackoverflow.com/questions/14894698/why-does-extern-const-int-n-not-work-as-expected)

[A](https://stackoverflow.com/a/14894897)

It's because `const` implies **internal linkage** by default, so your "definition" isn't visible outside of the translation unit where it appears.

In this case, by far the best solution is to put the declaration (`extern int const n;`) in a header file, and include that in both `a.cpp` and `b.cpp`. The linkage is determined by the first declaration the compiler sees, so the later definition in `a.cpp` will have the correct (external) linkage.

Alternatively, you can force the linkage in the definition:

```cpp
extern int const n = 8;
```

Despite the `extern`, this is still a definition; anything with an initializer outside of a class definition is a definition.

[A](https://stackoverflow.com/a/14894922)

`const` and `constexpr` variables in C++ have internal linkage (and thus aren't accessible in other compilation unit) if they aren't also declared `extern` (either in the definition or in a previous declaration).

In C, it isn't the case (well C hasn't `constexpr`) so your code is valid, and more you can put `extern` on a definition.

So if you want to write code which is both C and C++ (and the two declarations should probably come from the same header as James pointed out):

```cpp
// a.cpp
extern const int n;
const int n = 8;

// b.cpp
extern const int n;

int main()
{

    int m = n; 
}
```

if you don't

```cpp
// a.cpp
extern const int n = 8;
```

is also possible

### stackoverflow [Mixing extern and const](https://stackoverflow.com/questions/2190919/mixing-extern-and-const)



[A](https://stackoverflow.com/a/2190981)

- Yes, you can use them together.
- And yes, it should exactly match the declaration in the translation unit it's actually declared in. Unless of course you are participating in the [Underhanded C Programming Contest](http://www.underhanded-c.org/) :-)

The usual pattern is:

- *file.h:*
  `extern const int a_global_var;`
- *file.c:*
  `#include "file.h"`
  `const int a_global_var = /* some const expression */;`



### constexpr and linkage

这是c++11中引入的，那么它是否和`const`一样，是internal linkage呢？

参考：

- [use of constexpr in header file](https://stackoverflow.com/questions/50488831/use-of-constexpr-in-header-file)
- [Quick Q: use of constexpr in header file](https://isocpp.org/blog/2018/05/quick-q-use-of-constexpr-in-header-file)





## Example

在头文件`BussinessTypes.h`中，如下定义

```C++
namespace BussinessTypes
{
    
    int StockOpt = 1;
}

```

这种写法是非常不好的，当多个source file包含`BussinessTypes.h`，则会出现：multiple definition of `StockOpt`。

这让我想起来在头文件中声明的`const`常量不会出现这种问题，则将上述修改为

```C++
namespace BussinessTypes
{
    
    const int StockOpt = 1;
}
```

可以避免上述问题


这让我想起了`const`的linkage，显然它是internal linkage的。





