# Error prone

本文总结一些比较容易出错的情况。

## `++` used in macro function 

由于macro function所执行的仅仅是简单的replacement，所以当将`++`作为它的“入参”的时候，就可能导致容易出现错误的代码。

如下是一个例子：

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

上述代码会被替换为如下形式：

```c++
int main()
{
int x = 2;
int y = ((x++) * (x++));
std::cout << x << '\n'
          << y << '\n';

}

```



结果编译，运行结果如下：

```
4
6
```



`((x++) * (x++))`中第一个`(x++)`返回值为`2`，第二个`(x++)`的返回值为`3`。

## VS for `++i` `i++` VS while  `++i` `i++` 

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
while(i--){ // i--的返回值为i，所以它使用i的值来进行判断，该语句执行完成后i=i-1
    // 
    printf("%d\n", i);
}

printf("-------\n");
i = 5;
while(--i){ // 使用 i = i - 1的值进行判断，该语句执行完成后，i=i-1
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