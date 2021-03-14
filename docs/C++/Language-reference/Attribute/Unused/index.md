# Unused 



## stackoverflow [How do I best silence a warning about unused variables?](https://stackoverflow.com/questions/1486904/how-do-i-best-silence-a-warning-about-unused-variables)



### [A](https://stackoverflow.com/a/1486931)

You can [put it in "`(void)var;`" expression](https://stackoverflow.com/questions/308277/what-is-the-consequences-of-ignoring-warning-unused-parameter/308286#308286) (does nothing) so that a compiler sees it is used. This is portable between compilers.

E.g.

```C++
void foo(int param1, int param2)
{
    (void)param2;
    bar(param1);
}
```

Or,

```C++
#define UNUSED(expr) do { (void)(expr); } while (0)
...

void foo(int param1, int param2)
{
    UNUSED(param2);
    bar(param1);
}
```

#### Comment

I wonder how to do that for a variadic template. In `template<typename... Args> void f(const Args&... args)` I can't write `(void)args;` or `(void)args...;` because both are syntax errors. – [panzi](https://stackoverflow.com/users/277767/panzi) [May 18 '14 at 16:27](https://stackoverflow.com/questions/1486904/how-do-i-best-silence-a-warning-about-unused-variables/1486931#comment36464667_1486931)


@panzi : Use pack expansion, something like: `(void) (0 + ... + (args, 0))` – [Mark K Cowan](https://stackoverflow.com/users/1156377/mark-k-cowan) [Jan 16 '19 at 19:38](https://stackoverflow.com/questions/1486904/how-do-i-best-silence-a-warning-about-unused-variables/1486931#comment95274180_1486931)

@MarkKCowan Or just `((void)args, ...)`. – [L. F.](https://stackoverflow.com/users/9716597/l-f) [Jul 8 '19 at 11:34](https://stackoverflow.com/questions/1486904/how-do-i-best-silence-a-warning-about-unused-variables/1486931#comment100408451_1486931)

> NOTE: 
> 1、这是fold expression

### [A](https://stackoverflow.com/a/1487128)

In GCC and Clang you can use the `__attribute__((unused))` preprocessor directive to achieve your goal.
For example:

```C++
int foo (__attribute__((unused)) int bar) {
   return 0;
}
```



## Variadic template

在 stackoverflow [How do I best silence a warning about unused variables?](https://stackoverflow.com/questions/1486904/how-do-i-best-silence-a-warning-about-unused-variables) # [A](https://stackoverflow.com/a/1486931) 的comment中提及了这个topic，下面是完整的code: 

```C++
template<typename ... Args>
void f(const Args &... args)
{
	((void)args, ...);
}

int main()
{

}
// g++   --std=c++11 -Wall -pedantic -pthread main.cpp && ./a.out

```

编译输出如下:

```C++
main.cpp: In function 'void f(const Args& ...)':
main.cpp:4:15: warning: fold-expressions only available with '-std=c++17' or '-std=gnu++17'
    4 |  ((void)args, ...);
```



## TODO

1、jmmv [Unused parameters in C and C++](https://jmmv.dev/2015/02/unused-parameters-in-c-and-c.html)

2、stackoverflow [Unused parameter in c++11](https://stackoverflow.com/questions/15763937/unused-parameter-in-c11)