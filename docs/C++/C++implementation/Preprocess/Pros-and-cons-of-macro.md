# Pros and cons of macro

## Cons

本文讨论macro的劣势。

参考：

- [Why are preprocessor macros evil and what are the alternatives?](https://stackoverflow.com/questions/14041453/why-are-preprocessor-macros-evil-and-what-are-the-alternatives)
- [3 Types of Macros That Improve C++ Code](https://www.fluentcpp.com/2019/05/14/3-types-of-macros-that-improve-c-code/)



### It is hard to debug macros

When you have a macro that translates to a number or a string, the source code will have the macro name, and many debuggers, you can't "see" what the macro translates to. So you don't actually know what is going on.

**Replacement**: Use `enum` or `const T`

For "function-like" macros, because the debugger works on a "per source line where you are" level, your macro will act like a single **statement**, no matter if it's one statement or a hundred. Makes it hard to figure out what is going on.

**Replacement**: Use functions - inline if it needs to be "fast" (but beware that too much inline is not a good thing)

### Macro expansions can have strange side effects.

The famous one is `#define SQUARE(x) ((x) * (x))` and the use `x2 = SQUARE(x++)`. That leads to `x2 = (x++) * (x++);`, which, even if it was valid code [1], would almost certainly not be what the programmer wanted. If it was a function, it would be fine to do `x++`, and x would only increment once.

> [1] It is undefined behaviour to update one variable more than once "in a sequence point". A sequence point is not exactly the same as a statement, but for most intents and purposes, that's what we should consider it as. So doing `x++ * x++` will update `x` twice, which is undefined and will probably lead to different values on different systems, and different outcome value in `x` as well.

测试程序如下：

```c++
#include <iostream>
#define SQUARE(x) ((x) * (x))

int main()
{
int x = 2;
int y = SQUARE(x++);
std::cout << x << '\n'
          << y << '\n';

}
```

输出如下：

```
4
6
```



Another example is "if else" in macros, say we have this:

```cpp
#define safe_divide(res, x, y)   if (y != 0) res = x/y;
```

and then

```cpp
if (something) safe_divide(b, a, x);
else printf("Something is not set...");
```

It actually becomes completely the wrong thing....

**Replacement**: real functions.

测试程序如下：

```c
#include <stdio.h>
#define safe_divide(res, x, y)   if (y != 0) res = x/y;

int main()
{
int res = 0;
int x = 4, y=2;
if(true)
safe_divide(res, x, y);
else printf("Something is not set...");

}

```

编译报错如下：

```c
test.cpp:11:1: error: ‘else’ without a previous ‘if’
```

上诉错误是因为`safe_divide`的定义太过简单，一般的技巧是使用`do{}while(0)`包一层。

### Macros have no namespace

If we have a macro:

```cpp
#define begin() x = 0
```

and we have some code in `C++` that uses begin:

```cpp
std::vector<int> v;

... stuff is loaded into v ... 

for (std::vector<int>::iterator it = myvector.begin() ; it != myvector.end(); ++it)
   std::cout << ' ' << *it;
```

Now, what error messge do you think you get, and where do you look for an error [assuming you have completely forgotten - or didn't even know about - the begin macro that lives in some header file that someone else wrote? [and even more fun if you included that macro before the include - you'd be drowning in strange errors that makes absolutely no sense when you look at the code itself.

**Replacement**: Well there isn't so much as a replacement as a "rule" - only use uppercase names for macros, and never use all uppercase names for other things.

> NOTE: 这解释了为什么macro要使用upper case

### Macros have effects you don't realize

Take this function:

```cpp
#define begin() x = 0
#define end() x = 17
... a few thousand lines of stuff here ... 
void dostuff()
{
    int x = 7;

    begin();

    ... more code using x ... 

    printf("x=%d\n", x);

    end();

}
```

Now, without looking at the macro, you would think that begin is a function, which shouldn't affect x.

This sort of thing, and I've seen much more complex examples, can REALLY mess up your day!

**Replacement**: Either don't use a macro to set x, or pass x in as an argument.



## Pros

参考：

- https://stackoverflow.com/questions/96196/when-are-c-macros-beneficial



There are times when using macros is definitely beneficial. One example is to wrap a function with macros to pass on file/line information:

```cpp
#define malloc(x) my_debug_malloc(x, __FILE__, __LINE__)
#define free(x)  my_debug_free(x, __FILE__, __LINE__)
```

Now we can use `my_debug_malloc` as the regular malloc in the code, but it has extra arguments, so when it comes to the end and we scan the "which memory elements hasn't been freed", we can print where the allocation was made so the programmer can track down the leak.

