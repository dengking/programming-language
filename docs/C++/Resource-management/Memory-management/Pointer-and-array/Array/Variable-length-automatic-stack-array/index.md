# Variable-length automatic/stack array

1、在学习dynamic programming的文章**labuladong** [经动态规划：编辑距离](https://mp.weixin.qq.com/s/uWzSvWWI-bWAV3UANBtyOw) 时，为了实现其中的dp table，我专门学习了`Dynamic-multiple-dimension-array`，而在阅读 leetcode [72. 编辑距离](https://leetcode-cn.com/problems/edit-distance/) 中给出的如下标准答案: 

```C++
class Solution {
public:
    int minDistance(string word1, string word2) {
        int n = word1.length();
        int m = word2.length();

        // 有一个字符串为空串
        if (n * m == 0) return n + m;

        // DP 数组
        int D[n + 1][m + 1];

        // 边界状态初始化
        for (int i = 0; i < n + 1; i++) {
            D[i][0] = i;
        }
        for (int j = 0; j < m + 1; j++) {
            D[0][j] = j;
        }

        // 计算所有 DP 值
        for (int i = 1; i < n + 1; i++) {
            for (int j = 1; j < m + 1; j++) {
                int left = D[i - 1][j] + 1;
                int down = D[i][j - 1] + 1;
                int left_down = D[i - 1][j - 1];
                if (word1[i - 1] != word2[j - 1]) left_down += 1;
                D[i][j] = min(left, min(down, left_down));

            }
        }
        return D[n][m];
    }
};

```

乍一看` int D[n + 1][m + 1];// DP 数组` ，我感到比较错愕，这不就是multiple dimension array吗，显然它不是dynamic allocation的，它是stack array，C++支持dynamic stack array吗？



## C++标准并不支持dynamic stack array

1、cppreference [Array declaration](https://en.cppreference.com/w/cpp/language/array)

其中并没有dynamic stack array的介绍。

2、虽然标准并不支持，但是gcc是支持的

## people.eecs.ku.edu [Moving From Java to C++](https://people.eecs.ku.edu/~jrmiller/Courses/JavaToC++/StackAllocatedArrays.html)

### Stack-Allocated Arrays

Unlike Java, C++ arrays can be allocated on the stack. Java arrays are a special type of object, hence they can only be dynamically allocated via "`new`" and therefore allocated on the heap.

In C++, the following code is perfectly valid. The array "`localBuf`" will be allocated on the stack when `work` is called, and it will be discarded when `work` exits:

```C++
void work( )
{
    double localBuf[30];
    for (int i=0 ; i<30 ; i++)
        localBuf[i] = sqrt(i);
    …
    // as this function exits, localBuf is discarded along with the stack frame
}
```

**Be cautious** if you are trying to use "large" stack-allocated arrays. In most (and probably all) runtime systems, stack-allocated arrays will not get properly allocated if the memory required is larger than some system-dependent threshold. Programs that attempt to stack-allocate arrays requiring more than this threshold will therefore typically crash as soon as they try to use the array. Worse yet, you will receive no compilation error or warning; the program will just crash when run. The solution is to allocate large arrays on the heap instead, being sure to delete them when they are no longer needed. (Deletion of dynamically allocated memory is discussed in the **Memory management** section.)

> NOTE: 
>
> 1、stackoverflow

#### Dynamic stack array

Some C++ compilers allow arrays whose size is not known until runtime to be stack-allocated as well. For example:

```C++
void createAndUseAnArray(int size)
{
    double buf[size];
    useTheArray(buf, size);
    …
}
```

Like "`localBuf`" in the "`work`" function above, "`buf`" will be stack-allocated and deleted as "`createAndUseAnArray`" exits. While `g++` allows this, other compilers (e.g., the compilers in MS Visual Studio) do not. Moreover, Stroustrup's original specification of the language as well as most C++ standards specifications specify that the array size for a stack-allocated array must be a compile-time constant. For these reasons alone, stack allocating arrays whose size is not known at compile time is not recommended. Moreover, programs that unconditionally attempt to stack-allocate arrays whose size is not known until runtime can easily crash if this runtime-determined size turns out to be too large as just described above.

In summary, the fact that C++ allows stack-allocated arrays is a useful convenience for relatively small arrays whose size is known at compile time. For large arrays and/or arrays whose size is not known until runtime, you should always dynamically allocate them on the heap, explicitly deleting them when they are no longer needed.



## gcc [6.20 Arrays of Variable Length](https://gcc.gnu.org/onlinedocs/gcc/Variable-Length.html)

Variable-length automatic arrays are allowed in ISO C99, and as an extension GCC accepts them in C90 mode and in C++. These arrays are declared like any other automatic arrays, but with a length that is not a constant expression. The storage is allocated at the point of declaration and deallocated when the block scope containing the declaration exits. For example:

```c++
FILE *
concat_fopen (char *s1, char *s2, char *mode)
{
  char str[strlen (s1) + strlen (s2) + 1];
  strcpy (str, s1);
  strcat (str, s2);
  return fopen (str, mode);
}
```

## C++ Core Guidelines `gsl::stack_array`

### stackoverflow [stack_array in C++ Core Guidelines](https://stackoverflow.com/questions/41978790/stack-array-in-c-core-guidelines)

The C++ Core Guidelines mention something called a [`stack_array`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Res-stack). Its usage looks like:

```cpp
const int n = 7;
int m = 9;

void f()
{
    std::array<int, n> a1;
    stack_array<int> a2(m);  // A stack-allocated array.
                             // The number of elements are determined
                             // at construction and fixed thereafter.
    // ...
}
```

But how can such a class be implemented? How can we dynamically determine the stack size at run time?

#### [A](https://stackoverflow.com/a/41979333)

As far as I know, `stack_array` is a suggestion for a hypothetical class that can not be implemented using standard C++ (as of current standard). Its implementation requires (currently) non-standard compiler specific support, and I doubt such non-standard support even exists yet.

The closest you can get is a macro, that wraps a call to `alloca` (a non standard feature, which is supported by many compilers). See roalz's answer for a link to a concrete implementation. I'm not sure if that approach can achieve any safety that is not achievable by VLA (another non standard feature, which is supported by many compilers) - which is not to say that VLA are safe to use.

### CppCoreGuidelines [ES.27: Use `std::array` or `stack_array` for arrays on the stack](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Res-stack)

##### Reason

They are readable and don't implicitly convert to pointers. They are not confused with non-standard extensions of built-in arrays.

##### Example, bad

```
const int n = 7;
int m = 9;

void f()
{
    int a1[n];
    int a2[m];   // error: not ISO C++
    // ...
}
```

##### Note

The definition of `a1` is legal C++ and has always been. There is a lot of such code. It is error-prone, though, especially when the bound is non-local. Also, it is a "popular" source of errors (buffer overflow, pointers from array decay, etc.). The definition of `a2` is C but not C++ and is considered a security risk

##### Example

```
const int n = 7;
int m = 9;

void f()
{
    array<int, n> a1;
    stack_array<int> a2(m);
    // ...
}
```

##### Enforcement

- Flag arrays with non-constant bounds (C-style VLAs)
- Flag arrays with non-local constant bounds



## papers N3639. [Runtime-sized arrays with automatic storage duration](https://isocpp.org/files/papers/N3639.html) for c++