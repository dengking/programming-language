# Philosophy



## CppCoreGuidelines [Philosophy](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#p-philosophy)



> NOTE: 
>
> P.1 和 P.3 的含义比较接近，可以对两者进行总结，下面是我的一些想法: 
>
> 一、programming language也是一种[语言](https://dengking.github.io/Post/Language/Language/)，我们使用它来描述万事万物；我们需要使用programming language提供的feature来完整地的“描述”我们的意图，而不是将意图隐式地在comments 、document、甚至口头 说明。
>
> 这一点与[zen of python](https://www.python.org/dev/peps/pep-0020/)的
>
> ```
> Explicit is better than implicit.
> ```
>
> 二、How to do？
>
> 原文既给出了例子，也给处理how to do guideline，下面是我结合我的经验对how to do guideline的一些总结
>
> 典型例子: `std::chrono::seconds`
>
> 参见 `Strong-type` 章节
>
> 2、使用good abstraction
>
> 典型例子: `std::span` 等等
>
> 3、prefer library over hand-write code
>
> 尽可能地使用library；
>
> "Prefer algorithm over hand-write loop principle"，参见 `STL\Idiom\Guidelines`；
>
> 4、Use specifier to make it explicit principle
>
> C++ 提供了丰富的specifier，programmer应该使用它来准确地表达"intent"
>
> 5、Use language construct properly
>
> 合理地运用language construct，能够使得program更加准确、少些code

### P.1: Express ideas directly in code

**Reason** Compilers don’t read comments (or design documents) and neither do many programmers (consistently). What is expressed in code has defined semantics and can (in principle) be checked by compilers and other tools.



#### Example: strong type +Use specifier

```C++
class Date {
public:
    Month month() const;  // do
    int month();          // don't
    // ...
};
```

The first declaration of `month` is explicit about returning a `Month` and about not modifying the state of the `Date` object. The second version leaves the reader guessing and opens more possibilities for uncaught bugs.

> NOTE: 
>
> 上述example说明了: 
>
> 1、strong type
>
> 2、use specifier
>
> C++提供了丰富的specifier，programmer应该使用这些specifier来使program更加准确

#### Example: Prefer algorithm over hand-write loop

##### Example, bad

 This loop is a restricted form of `std::find`:

```c++
void f(vector<string>& v)
{
    string val;
    cin >> val;
    // ...
    int index = -1;                    // bad, plus should use gsl::index
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] == val) {
            index = i;
            break;
        }
    }
    // ...
}
```

##### Example, good

 A much clearer expression of intent would be:

```C++
void f(vector<string>& v)
{
    string val;
    cin >> val;
    // ...
    auto p = find(begin(v), end(v), val);  // better
    // ...
}
```

A well-designed library expresses intent (what is to be done, rather than just how something is being done) far better than direct use of language features.

> NOTE: 
>
> 1、上述对比的例子体现了: Prefer algorithm over hand-write loop
>
> 2、推广: prefer library over hand-write code

#### Example: strong type 

```c++
change_speed(double s);  // bad: what does s signify?
// ...
change_speed(2.3);
```

```c++
change_speed(Speed s);  // better: the meaning of s is specified
// ...
change_speed(2.3);    // error: no unit
change_speed(23m / 10s); // meters per second
```



### P.3: Express intent

**Reason** Unless the intent of some code is stated (e.g., in names or comments), it is impossible to tell whether the code does what it is supposed to do.

> NOTE: 
>
> 1、上述“stated”的含义是“说明、描述、表述”
>
> 2、在阅读维基百科[smart pointer](https://en.wikipedia.org/wiki/Smart_pointer)时，其中提及了smart pointer属于[intentional programming](https://en.wikipedia.org/wiki/Intentional_programming)，显然smart pointer是典型的express intent。

#### Example: Use language construct properly + prefer library

> NOTE: 
>
> 1、虽然 while 也能够实现 iteration，但是当仅仅是表达for each的时候，后面的range-for、`std::for_each`显然更加合适，显然programmer需要"Use language construct properly"

```C++
gsl::index i = 0;
while (i < v.size()) {
    // ... do something with v[i] ...
}
```

The intent of “just” looping over the elements of `v` is not expressed here. The implementation detail of an index is exposed (so that it might be misused), and `i` outlives the scope of the loop, which might or might not be intended. The reader cannot know from just this section of code.

Better:

```
for (const auto& x : v) { /* do something with the value of x */ }
```

Now, there is no explicit mention of the iteration mechanism, and the loop operates on a reference to `const` elements so that accidental modification cannot happen. If modification is desired, say so:

```
for (auto& x : v) { /* modify x */ }
```

For more details about for-statements, see [ES.71](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-for-range). Sometimes better still, use a named algorithm. This example uses the `for_each` from the Ranges TS because it directly expresses the intent:

```
for_each(v, [](int x) { /* do something with the value of x */ });
for_each(par, v, [](int x) { /* do something with the value of x */ });
```

The last variant makes it clear that we are not interested in the order in which the elements of `v` are handled.

A programmer should be familiar with

- [The guidelines support library](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-gsl)
- [The ISO C++ Standard Library](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-stdlib)
- Whatever foundation libraries are used for the current project(s)



#### Example: strong type + create good abstraction

 If two `int`s are meant to be the coordinates of a 2D point, say so:

```C++
draw_line(int, int, int, int);  // obscure
draw_line(Point, Point);        // clearer
```



### P.2: Write in ISO Standard C++

> NOTE: 
>
> 1、避免undefined behavior、implementation-defined behavior

### P.4: Ideally, a program should be statically type safe

**Reason** Ideally, a program would be completely statically (compile-time) type safe. Unfortunately, that is
not possible. Problem areas:

1、unions

2、casts

3、array decay

> NOTE: 
>
> array decay就导致失去了长度信息

4、range errors

5、narrowing conversions

#### Note

These areas are sources of serious problems (e.g., crashes and security violations). We try to provide alternative techniques.

#### Enforcement

We can ban, restrain, or detect the individual problem categories separately, as required and feasible for individual programs. Always suggest an alternative. For example:

1、unions – use `variant` (in C++17)

2、casts – minimize their use; templates can help

> NOTE: 
>
> C++的meta-programming 技术，赋予了programmer非常强大的、灵活的对类型进行描述、检查的能力，一旦类型不符合预期，则立即给出编译报错。
>

3、array decay – use `span` (from the GSL)

4、range errors – use `span`

5、narrowing conversions – minimize their use and use `narrow` or `narrow_cast` (from the GSL) where they are necessary

> NOTE: 
>
> 一、上述这些问题，C++基于C努力地进行着改进，这在 `Comparison-of-programming-language\C++VS-C\Type-system` 章节 中进行了总结。
>
> 二、关于type safety，参见 `Theory\Type-system\Type-safety` 章节
>
> 

### P.8: Don’t leak any resources

> NOTE: 
>
> 参见: 
>
> 1、CppCoreGuidelines [The resource management section](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource)
>
> 2、`Resource-management` 章节

#### Note

 Enforcing [the lifetime safety profile](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#SS-lifetime) eliminates leaks. When combined with resource safety provided by [RAII](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-raii), it eliminates the need for “garbage collection” (by generating no garbage). Combine this with enforcement of [the type and bounds profiles](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#SS-force) and you get complete type- and resource-safety, guaranteed by tools.

> NOTE: 
>
> 1、我是因为看到了"  [the lifetime safety profile](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#SS-lifetime) " 这段话而将上面这段话摘抄下来的，关于object lifetime，一种典型的错误就是access outside of object lifetime，这在"Access-outside-of-object-lifetime"章节进行了总结。

### P.9: Don’t waste time or space

> NOTE: 
>
> 1、optimization principle
>
> 2、Zero-overhead principle-You don't pay for what you don't use

### P.11: Encapsulate messy constructs, rather than spreading through the code

> NOTE: 
>
> 1、其实就是合理的encapsulation