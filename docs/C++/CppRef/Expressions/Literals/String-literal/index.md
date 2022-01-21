# string literal

本文的内容是整合cppreference c++ [string literal](https://en.cppreference.com/w/cpp/language/string_literal)和cppreference c [string literals](https://en.cppreference.com/w/c/language/string_literal)。



## cppreference c++ [string literal](https://en.cppreference.com/w/cpp/language/string_literal)和cppreference c [string literals](https://en.cppreference.com/w/c/language/string_literal)

### Notes

The null character ('**\0**', L'**\0**', `char16_t()`, etc) is always appended to the string literal: thus, a string literal `"Hello"` is a `const char[6]` holding the characters 'H', 'e', 'l', 'l', 'o', and '**\0**'.

> NOTE: 这个规则非常重要，在后面的String literal and array中会对此进行展开



String literals placed side-by-side are concatenated at [translation phase 6](https://en.cppreference.com/w/cpp/language/translation_phases) (after the preprocessor). That is, "Hello," " world!" yields the (single) string "Hello, world!". If the two strings have the same encoding prefix (or neither has one), the resulting string will have the same encoding prefix (or no prefix).

> NOTE: 这种安排就允许了在string literal concat as one中所描述的场景。



String literals have [static storage duration](https://en.cppreference.com/w/cpp/language/storage_duration), and thus exist in memory for the life of the program.

String literals can be used to [initialize character arrays](https://en.cppreference.com/w/cpp/language/aggregate_initialization). If an array is initialized like char `str[] = "foo"`;, `str` will contain a copy of the string `"foo"`.



## String literal and array

一提及string literal，我们第一想法就是：它表示的是string，其实除此之外，string literal还有其他的用途：

cppreference c++ [string literal](https://en.cppreference.com/w/cpp/language/string_literal)中描述：

> String literals can be used to [initialize character arrays](https://en.cppreference.com/w/cpp/language/aggregate_initialization).

在cppreference c [string literals](https://en.cppreference.com/w/c/language/string_literal)中，有这样的描述：

> A string literal is not necessarily a string; if a string literal has embedded null characters, it represents an array which contains more than one string
>
> ```c
> char* p = "abc\0def"; // strlen(p) == 3, but the array has size 8
> ```
>
> > NOTE: 需要注意的是，这表示的是一个array。在下面会结合具体例子对此进行说明。

### array的长度和string的长度



String literal and array中的一个重要问题是：array的长度和string的长度，其实这个问题可以表述为：`sizeof`和`strlen`。

下面结合具体的例子来进行说明：

在cppreference c [string literals](https://en.cppreference.com/w/c/language/string_literal)中给出的例子：

```c++
const char* p = "abc\0def"; // std::strlen(p) == 3, but the array has size 8
```



```c
char a1[] = "abc"; // a1 is char[4] holding {'a', 'b', 'c', '\0'}
char a2[4] = "abc"; // a2 is char[4] holding {'a', 'b', 'c', '\0'}
char a3[3] = "abc"; // a3 is char[3] holding {'a', 'b', 'c'}
```

另外可以参见：

https://stackoverflow.com/questions/4711449/what-does-the-symbol-0-mean-in-a-string-literal

```
char str[] = "Hello\0"; // sizeof str is 7 ，strlen(str)
```



### `char a3[3] = "abc";`

上面的例子中的，`char a3[3] = "abc";`，可能无法编译通过的，写了一个小程序：

```c
#include <iostream>
#include <string>
#include <string.h>

int main() {

char a3[3] = "abc";
std::cout<<strlen(a3)<<std::endl;
}

```

报错如下：

```
error: initializer-string for array of chars is too long [-fpermissive]
 char a3[3] = "abc";
```

这是因为，存储string literal `"abc"`是需要4个字节的空间。

### Initialize array

上一节已经讨论了`char a3[3] = "abc";`的问题，下面是准确的写法：

```C++
#include <iostream>
#include <string>
#include <string.h>

int main() {

char a3[3] = {'a', 'b', 'c'};
std::cout<<strlen(a3)<<std::endl;
}
```

### 已知字符串长度，使用`char array`来进行保存

如果已知字符串长度`s`，则声明数组的长度为`s+1`，需要留下一个字节来保存`\0`。

### `char amessage[]`  VS `char *pmessage` 

```c
char amessage[] = "now is the time";
char *pmessage = "now is the time";
```

`char amessage[] = "now is the time"`表示：使用string literal `"now is the time"`来初始化数组`amessage`。

`char *pmessage = "now is the time";`表示：`pmessage` 是一个指向string literal `"now is the time"`的指针。

参考内容：

[C: differences between char pointer and array](https://stackoverflow.com/questions/1335786/c-differences-between-char-pointer-and-array)

[Why do I get a segmentation fault when writing to a string initialized with “char *s” but not “char s”?](https://stackoverflow.com/questions/164194/why-do-i-get-a-segmentation-fault-when-writing-to-a-string-initialized-with-cha)

[char *array and char array[]](https://stackoverflow.com/questions/20347170/char-array-and-char-array)

## String literal concat as one

关于这个问题，参见：

[String Literal Concatenation](https://docs.microsoft.com/en-us/cpp/c-language/string-literal-concatenation?view=vs-2019)

https://www.techiedelight.com/concatenate-string-literals-c-cpp/

[Concatenate two string literals](https://stackoverflow.com/questions/6061648/concatenate-two-string-literals)

## String literal represents an array containing more than one string

在阅读代码的时候发现了下面这种写法：

```c++
#define SID_CLOUD_DLOG "cloud.dlog"
#define SID_ESB_MESSAGEFACTORY "..."
...

return SID_CLOUD_DLOG"\0"SID_ESB_MESSAGEFACTORY"\0"SID_F2PACKSVR"\0"SID_CHANNEL_GE"\0"SID_CFGSVR"\0\0";

```

上述代码所展示的就是“an array which contains more than one string”。



下面是一个可以运行的测试程序

```c++
#include <iostream>
#include <vector>
#include <string>

#define SID_CLOUD_DLOG "cloud.dlog"
#define SID_ESB_MESSAGEFACTORY "com.hundsun.fbase.esbmessagefactory"

const char* GetHQDependSvr()
{
    return SID_CLOUD_DLOG"\0"SID_ESB_MESSAGEFACTORY"\0\0";
}

int main() {
std::string s = GetHQDependSvr();
std::cout<<s<<std::endl;

}
```

运行结果如下：

```
cloud.dlog
```



`SID_CLOUD_DLOG"\0"SID_ESB_MESSAGEFACTORY"\0\0"`结果preprocessor处理后，给替换为:

```c++
"cloud.dlog\0com.hundsun.fbase.esbmessagefactory\0\0"
```

显然它是一个string literal，在c++中，它的类型为`const char *`，所以函数`const char* GetHQDependSvr()`的返回值类型为`const  char *`。



需要注意的是，`SID_CLOUD_DLOG"\0"SID_ESB_MESSAGEFACTORY"\0\0"`是一个String literal represents an array containing more than one string。

## Escape

在string literal中，`\`表示escape，所以`\0`就表示的escape。

## [Null character](https://en.wikipedia.org/wiki/Null_character)

