# If Statements with Initializer (since C++17)



一、其实这种用法和  `for(int i=0;i<count;++i)`  中定义`i`是一样的，这是我在阅读 stackoverflow [Variable assignment in an "if" condition](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition) # [A](https://stackoverflow.com/a/17681782/10173843) 中的如下comment发现的:

> Putting the definition in the condition tightens the scope so that you don't accidentally refer to `derived` on the next line after the if-statement. It's the same reason we use `for (int i = 0...` instead of `int i; for (i = 0; ...`. 
>
> – [Adrian McCarthy](https://stackoverflow.com/users/1386054/adrian-mccarthy)  [Jul 16, 2013 at 16:36](



二、stackoverflow [Variable assignment in an "if" condition](https://stackoverflow.com/questions/17681535/variable-assignment-in-an-if-condition) # [A](https://stackoverflow.com/a/67281305/10173843) 

> In [C++17](https://en.wikipedia.org/wiki/C%2B%2B17), one can use:
>
> ```cpp
> if (<initialize> ; <conditional_expression>) { <body> }
> ```
>
> Similar to a *for* loop iterator initializer.
>
> Here is an example:
>
> ```cpp
> if (Employee employee = GetEmployee(); employee.salary > 100) { ... }
> ```



三、tutorialspoint [C++17 If statement with initializer](https://www.tutorialspoint.com/cplusplus17-if-statement-with-initializer)

> ```c++
> #include <iostream>
> #include <cstdlib>
> using namespace std;
> 
> int main()
> {
>     srand(time(NULL));
>     // C++17 if statement with initializer
>     if (int random_num = rand(); random_num % 2 == 0)
>     {
>         cout << random_num << " is an even number\n";
>     }
>     else
>     {
>         cout << random_num << " is an odd number\n";
>     }
>     return 0;
> }
> ```



四、coders-corner [C++17: initializers in if-statement and switch-statement](https://coders-corner.net/2019/06/22/c17-initializers-in-if-statement-and-switch-statement/)

With C++17 it is possible to initialize a variable inside an if-statement and a switch-statement. 

```c++
// init inside if
if (int count = CalcCount(); count > 100)
{
    std::cout.."count: "..count..std::endl;
}

// init outside if
int count = CalcCount();

if (count > 100)
{
    std::cout.."count: "..count..std::endl;
}

// init outside if without line break
int count = CalcCount(); if (count > 100)
{
    std::cout.."count: "..count..std::endl;
}

```



```c++

// init inside if
if (int count = IsInitialized() ? CalcCount() : (CalcExpectedCount() + CalcOldCount()) / 2; count > 100)
{
    std::cout.."count: "..count..std::endl;
}

```



## cppreference [if statement](https://en.cppreference.com/w/cpp/language/if) # If Statements with Initializer (since C++17)

> NOTE:
>
> 一、skebanga [C++17 If statement with initializer](https://skebanga.github.io/if-with-initializer/)
>
> > Introduced under proposal [P00305r0](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0305r0.html), **If statement with initializer** give us the ability to initialize a variable within an `if` statement, and then, once initialized, perform the actual conditional check.
>
> 

```c++
#include <map>
#include <mutex>
#include <iostream>

std::map<int, std::string> m;
std::mutex mx;
extern bool shared_flag; // guarded by mx

int demo()
{
    //
    if (auto it = m.find(10); it != m.end())
    {
        return it->second.size();
    }
    //
    if (char buf[10]; std::fgets(buf, 10, stdin))
    {
        m[0] += buf;
    }
    // 其实init_statement不一定是对condition variable的初始化，从下面来看，它是非常灵活的
    if (std::lock_guard lock(mx); shared_flag)
    {
        unsafe_ping();
        shared_flag = false;
    }
    //
    if (int s; int count = ReadBytesWithSignal(&s))
    {
        publish(count);
        raise(s);
    }
    // 
    if (const auto keywords = {"if", "for", "while"};
        std::ranges::any_of(keywords, [&tok](const char *kw)
                            { return tok == kw; }))
    {
        std::cerr << "Token must not be a keyword\n";
    }
}
int main()
{
}
```

