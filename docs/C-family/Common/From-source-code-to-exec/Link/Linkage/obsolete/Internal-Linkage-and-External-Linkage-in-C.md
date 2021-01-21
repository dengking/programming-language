# geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)

It is often quite hard to distinguish between **scope** and **linkage**, and the roles they play. This article focuses on [scope](https://en.wikipedia.org/wiki/Scope_(computer_science)) and [linkage](https://en.wikipedia.org/wiki/Linkage_(software)), and how they are used in C language.

> NOTE : 关于linkage和scope，在[Translation unit (programming)](https://en.wikipedia.org/wiki/Translation_unit_(programming))中有描述；

**Note:** All C programs have been compiled on 64 bit GCC 4.9.2. Also, the terms “identifier” and “name” have been used interchangeably in this article.

## Definitions

- **Scope :** Scope of an identifier is the part of the program where the identifier may directly be accessible. In C, all identifiers are lexically (or statically) scoped.

- **Linkage :** Linkage describes how names can or can not refer to the same entity throughout the whole program or one single translation unit.

  ***SUMMARY*** : 这句话是抄自 [linkage](https://en.wikipedia.org/wiki/Linkage_(software))；

  The above sounds similar to Scope, but it is not so. To understand what the above means, let us dig deeper into the compilation process.

- [Translation Unit](https://en.wikipedia.org/wiki/Translation_unit_(programming)) : A translation unit is a file containing source code, header files and other dependencies. All of these sources are grouped together in a file for they are used to produce one single executable object. It is important to link the sources together in a meaningful way. For example, the compiler should know that `printf` definition lies in `stdio` header file.



**There are 2 types of linkage:**

## Internal Linkage

**Internal Linkage**: An identifier implementing **internal linkage** is not accessible outside the **translation unit** it is declared in. Any identifier within the unit can access an identifier having internal linkage. It is implemented by the keyword `static`. An internally linked identifier is stored in initialized or uninitialized segment of RAM. (**note:** `static ` also has a meaning in reference to scope, but that is not discussed here).

Some Examples:

```CQL
// animal.cpp
// C code to illustrate Internal Linkage 
#include <stdio.h> 

static int animals = 8; 
const int i = 5; 

int call_me(void) 
{ 
	printf("%d %d", i, animals); 
} 

```

The above code implements **static linkage** on identifier `animals`. Consider `Feed.cpp` is located in the same translation unit.

```c
// Feed.cpp
// C code to illustrate Internal Linkage 
#include <stdio.h> 

int main() 
{ 
	call_me(); 
	animals = 2; 
	printf("%d", animals); 
	return 0; 
} 

```

On compiling `Animals.cpp` first and then `Feed.cpp`, we get

```
Output : 5 8 2
```

***SUMMARY*** : 上面`Feed.cpp`是有误的，因为它在使用`call_me`和`animals`之前并没有declare它们，所以编译的时候是会报如下错误的：

```c
[quotepredict@localhost test-third-party]$ g++ Feed.cpp animal.cpp 
Feed.cpp: 在函数‘int main()’中:
Feed.cpp:7:10: 错误：‘call_me’在此作用域中尚未声明
  call_me(); 
          ^
Feed.cpp:8:2: 错误：‘animals’在此作用域中尚未声明
  animals = 2; 
  ^

```



Now, consider that `Feed.cpp` is located in a different translation unit. It will compile and run as above only if we use `#include "Animals.cpp"`.

Consider `Wash.cpp` located in a 3rd translation unit.

```c
// Wash.cpp
// C code to illustrate Internal Linkage 
#include <stdio.h> 
#include "animal.cpp" // note that animal is included. 

int main() 
{ 
	call_me(); 
	printf("\n having fun washing!"); 
	animals = 10; 
	printf("%d\n", animals); 
	return 0; 
} 

```

On compiling, we get:

```
Output : 5 8
having fun washing!
10
```

***SUMMARY*** : 在编译`Wash.cpp`时候，其实是并不涉及linker的，因为`#include "animal.cpp"`，preprocessor会将`animal.cpp`包到 `Wash.cpp`中；

There are 3 translation units (Animals, Feed, Wash) which are using `animals` code. This leads us to conclude that each **translation unit** accesses it’s own copy of `animals`. That is why we have `animals` = 8 for `Animals.cpp`, `animals` = 2 for `Feed.cpp` and `animals` = 10 for `Wash.cpp`. A file. This behavior eats up memory and decreases performance.

Another property of **internal linkage** is that it is **only implemented when the variable has global scope**, and all **constants** are by default internally linked.

***SUMMARY*** : 上面这段话中的最后一句话所说的是`C++`，而不是C，在[C++ Storage-class specifiers](https://en.cppreference.com/w/c/language/storage_duration)中对此有专门的对比；

下面是例子：

```c

// animal.cpp
// C code to illustrate Internal Linkage 
#include <stdio.h> 

static int animals = 8; 
const int i = 5; 

int call_me(void) 
{ 
	printf("%d %d\n", i, animals); 
	printf("%p %p\n", &i, &animals); 
} 
```

```c++
// Feed.cpp
// C code to illustrate Internal Linkage 
#include <stdio.h> 
extern int i;
int main() 
{ 
	printf("%d", i); 
	return 0; 
}
```

编译结果如下：

```
g++ Feed.cpp animal.cpp 
/tmp/ccViBb3h.o：在函数‘main’中：
Feed.cpp:(.text+0x6)：对‘i’未定义的引用
collect2: 错误：ld 返回 1

```



**Usage :** As we know, an internally linked variable is **passed by copy**. Thus, if a header file has a function `fun1()` and the source code in which it is included in also has `fun1()` but with a **different definition**, then the 2 functions will not clash with each other. Thus, we commonly use **internal linkage** to hide **translation-unit-local helper functions** from the **global scope**. For example, we might include a header file that contains a method to read input from the user, in a file that may describe another method to read input from the user. Both of these functions are independent of each other when linked.

***SUMMARY*** : Thus, we commonly use **internal linkage** to hide **translation-unit-local helper functions** from the **global scope**. 其实是使用`static`来修饰函数，这样这个函数的linkage就是internal了；

## External Linkage

**External Linkage:** An identifier implementing external linkage is visible to . Externally linked identifiers are between translation units and are considered to be located at the outermost level of the program. In practice, this means that you must define an identifier in a place which is visible to all, such that it has **only one visible definition**. It is the default linkage for globally scoped variables and functions. Thus, all instances of a particular identifier with **external linkage** refer to the same identifier in the program. The keyword  implements external linkage.

When we use the keyword `extern`, we tell the **linker** to look for the definition elsewhere. Thus, the declaration of an externally linked identifier does not take up any space. `Extern` identifiers are generally stored in initialized/uninitialized or text segment of RAM.

**Please do go through [Understanding extern keyword in](https://www.geeksforgeeks.org/understanding-extern-keyword-in-c/) C before proceeding to the following examples.** 

```c
// C code to illustrate External Linkage 
#include <stdio.h> 

void foo() 
{ 
	int a; 
	extern int b; // line 1 
} 

void bar() 
{ 
	int c; 
	c = b; // error 
} 

int main() 
{ 
	foo(); 
	bar(); 
} 

```

```
Error: 'b' was not declared in this scope
```

**Explanation :** The variable `b` has local scope in the function `foo`, even though it is an `extern` variable. Note that compilation takes place before linking; i.e **scope** is a concept that can be used only during compile phase. After the program is compiled there is no such concept as “scope of variable”.

During compilation, scope of `b` is considered. It has local scope in `foo()`. When the compiler sees the `extern` declaration, it trusts that there is a definition of `b` somewhere and lets the **linker** handle the rest.

However, the same compiler will go through the `bar()` function and try to find variable `b`. Since `b` has been declared `extern`, it has not been given memory yet by the compiler; it does not exist yet. The compiler will let the linker find the definition of `b` in the translation unit, and then the linker will assign `b` the value specified in definition. It is only then that `b` will **exist** and be assigned memory. However, since there is no declaration given at **compile time** within the scope of `bar()`, or even in global scope, the compiler complains with the error above.

Given that it is the compiler’s job to make sure that all variables are used within their scopes, it complains when it sees `b` in `bar()`, when `b` has been declared in `foo()`‘s scope. The compiler will stop compiling and the program will not be passed to the linker.

We can fix the program by declaring `b` as a global variable, by moving line 1 to before `foo`‘s definition.

Let us look at another example

```c
// C code to illustrate External Linkage 
#include <stdio.h> 

int x = 10; 
int z = 5; 

int main() 
{ 

	extern int y; // line 2 
	extern int z; 
	printf("%d %d %d", x, y, z); 
} 

int y = 2; 

```

We can explain the output by observing behaviour of **external linkage**. We define 2 variables `x` and `z` in *global* scope. By default, both of them have **external linkage**. Now, when we declare `y` as `extern`, we tell the compiler that there exists a `y` with some definition within the same translation unit. Note that this is during the compile time phase, where the compiler trusts the `extern`keyword and compiles the rest of the program. The next line, `extern int z `has no effect on `z`, as `z `is externally linked by default when we declared it as a **global variable** outside the program. When we encounter `printf` line, the compiler sees 3 variables, all 3 having been declared before, and all 3 being used within their scopes (in the `printf` function). The program thus compiles successfully, even though the compiler does not know the definition of `y`

The next phase is **linking**. The **linker** goes through the compiled code and finds `x` and `z` first. As they are global variables, they are externally linked by default. The linker then updates value of `x` and `z` throughout the entire translation unit as 10 and 5. If there are any references to `x` and `z` in any other file in the translation unit, they are set to 10 and 5.

Now, the linker comes to `extern int y` and tries to find any definition of `y` within the translation unit. It looks through every file in the translation unit to find definition of `y`. If it does not find any definition, a linker error will be thrown. In our program, we have given the definition outside `main()`, which has already been compiled for us. Thus, the linker finds that definition and updates `y`.



