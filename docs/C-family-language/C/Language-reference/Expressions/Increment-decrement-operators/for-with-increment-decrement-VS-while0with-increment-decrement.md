

# [C: What is the difference between ++i and i++?](https://stackoverflow.com/questions/24853/c-what-is-the-difference-between-i-and-i)



## [A](https://stackoverflow.com/a/24858)

- `++i` will increment the value of `i`, and then return the incremented value.

  ```c
   i = 1;
   j = ++i;
   (i is 2, j is 2)
  ```

- `i++` will increment the value of `i`, but return the original value that `i` held before being incremented.

  ```c
   i = 1;
   j = i++;
   (i is 2, j is 1)
  ```

For a `for` loop, either works. `++i` seems more common, perhaps because that is what is used in [K&R](https://en.wikipedia.org/wiki/The_C_Programming_Language).

In any case, follow the guideline "prefer `++i` over `i++`" and you won't go wrong.

There's a couple of comments regarding the efficiency of `++i` and `i++`. In any non-student-project compiler, there will be no performance difference. You can verify this by looking at the generated code, which will be identical.

The efficiency question is interesting... here's my attempt at an answer: [Is there a performance difference between i++ and ++i in C?](https://stackoverflow.com/questions/24886/is-there-a-performance-difference-between-i-and-i)

As *On Freund* notes, it's different for a C++ object, since `operator++()` is a function and the compiler can't know to optimize away the creation of a temporary object to hold the intermediate value.



## [A](https://stackoverflow.com/a/24874)

The reason `++i` *can* be slightly faster than `i++` is that `i++` can require a local copy of the value of i before it gets incremented, while `++i` never does. In some cases, some compilers will optimize it away if possible... but it's not always possible, and not all compilers do this.

I try not to rely too much on compilers optimizations, so I'd follow Ryan Fox's advice: when I can use both, I use `++i`.





# VS for `++i` `i++` VS while  `++i` `i++` 

正如上面所述的，`for(int i=0;i<5;i++)`与`for(int i=0;i<5;++i)`在循环次数、循环体中i的值方面都没有任何差异；以下是测试程序：

```c
#include <stdio.h>
#include <string.h>

int main(void) {
int i = 5;

printf("-------\n");
for(i = 0;i<5;++i){
    printf("%d\n", i);
}
printf("-------\n");
for(i = 0;i<5;i++){
    printf("%d\n", i);
}
}
```

但是对于while，`while(--i)`和`while(i--)`却有着非常大的区别，它的区别其实正是prefix 和 postfix的差异的体现；以下是测试程序：

```c
#include <stdio.h>
#include <string.h>

int main(void) {
int i = 5;

printf("-------\n");
while(i--){
    printf("%d\n", i);
}

printf("-------\n");
i = 5;
while(--i){
    printf("%d\n", i);
}
}
```

程序输出如下：

```
-------
4
3
2
1
0
-------
4
3
2
1

```

两者之间的差异主要源于：`for`和`while`的执行流程不同，但是两种情况都涉及到了[Order of evaluation](https://en.cppreference.com/w/c/language/eval_order)问题；