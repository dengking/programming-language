# Prevent optimization

有的时候，我们想阻止compiler的optimization，关于此，在 aristeia [C++ and the Perils of Double-Checked Locking](https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf) 中进行了非常好的总结。从这篇文章的详尽的分析来看，作者是不建议programmer执行prevent optimization的，但是在一些特殊的情况下，我们还是想要prevent optimization，典型的场景就是: benchmark，因此，本文总结一些Prevent optimization的技巧，由于它们不是任何情况下都有效的，因此，programmer需要进行谨慎的使用。

总的来说，有如下technique:

1、"indicate that value might be used late"，参见:

stackoverflow [Force compiler to not optimize side-effect-less statements](https://stackoverflow.com/questions/1152333/force-compiler-to-not-optimize-side-effect-less-statements) # [A](https://stackoverflow.com/a/1152396)

2、`volatile` variable

stackoverflow [Force compiler to not optimize side-effect-less statements](https://stackoverflow.com/questions/1152333/force-compiler-to-not-optimize-side-effect-less-statements) # [A](https://stackoverflow.com/a/1152574)

stackoverflow [How to correctly benchmark a [templated] C++ program](https://stackoverflow.com/questions/435627/how-to-correctly-benchmark-a-templated-c-program) # [A](https://stackoverflow.com/a/439278)

在 aristeia [C++ and the Perils of Double-Checked Locking](https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf) 中，对这种方法进行了详细的分析。

## aristeia [C++ and the Perils of Double-Checked Locking](https://www.aristeia.com/Papers/DDJ_Jul_Aug_2004_revised.pdf) 





## stackoverflow [Force compiler to not optimize side-effect-less statements](https://stackoverflow.com/questions/1152333/force-compiler-to-not-optimize-side-effect-less-statements)

> NOTE: 总结了一些阻止compiler optimization的技巧，但是，这些技巧是无法保证有效的。

[A](https://stackoverflow.com/a/1152396)

Compilers are unfortunately allowed to optimise as much as they like, even without any explicit switches, if the code behaves as if no optimisation takes place. However, you can often **trick** them into not doing so if you indicate that value might be used later, so I would change your code to:

```cpp
int float_to_int(float f)
{
    return static_cast<int>(f); // has no side-effects
}
```

As others have suggested, you will need to examine the assemnler output to check that this approach actually works.

> NOTE: 最终还是要通过查看assembly来确定是否生效。

[A](https://stackoverflow.com/a/1152574)

Assignment to a `volatile` variable shold never be optimized away, so this might give you the result you want:

```cpp
static volatile int i = 0;

void float_to_int(float f)
{
    i = static_cast<int>(f); // has no side-effects
}
```



## stackoverflow [How to correctly benchmark a [templated] C++ program](https://stackoverflow.com/questions/435627/how-to-correctly-benchmark-a-templated-c-program)

> NOTE: 提问者的问题总结起来其实非常简单: 他想要进行benchmark，但是它的benchmark code被optimization out了，因此，他想要知道如何来避免这些benchmark code被optimization out

[A](https://stackoverflow.com/a/439278)

If you want to force *any* compiler to not discard a result, have it write the result to a volatile object. That operation cannot be optimized out, by definition.

```cpp
template<typename T> void sink(T const& t) {
   volatile T sinkhole = t;
}
```

No `iostream` overhead, just a copy that has to remain in the generated code. Now, if you're collecting results from a lot of operations, it's best not to discard them one by one. These copies can still add some overhead. Instead, somehow collect all results in a single non-volatile object (so all individual results are needed) and then assign that result object to a volatile. E.g. if your individual operations all produce strings, you can force evaluation by adding all char values together modulo 1<<32. This adds hardly any overhead; the strings will likely be in cache. The result of the addition will subsequently be assigned-to-volatile so each char in each sting must in fact be calculated, no shortcuts allowed.

