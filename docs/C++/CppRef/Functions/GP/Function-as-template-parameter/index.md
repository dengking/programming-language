# Function as template parameter

"function as template parameter"是function generic programming的核心所在；

支持function call的: 

1、pointer to function

2、pointer to member 

3、functor

1和3的syntax是一致的，但是2的syntax是不同的；

## Examples

1、[SergiusTheBest](https://github.com/SergiusTheBest)/[ScopeExit](https://github.com/SergiusTheBest/ScopeExit)

```C++
template<typename F>
class ScopeExit 
{
public:
    explicit ScopeExit(F&& fn) : m_fn(fn)
private:
    F m_fn;
};
```



2、[std::thread::thread](https://en.cppreference.com/w/cpp/thread/thread/thread)

> TODO: 思考一个问题: `std::thread`并不是template，它是如何实现generic的？`std::thread`是底层thread的wrapper，它会将constructor入参转换为function pointer；关于实现的code，function to pointer，显然这是decay。
>
> 这非常重要，如何实现转换。显然，其中之一就是`std::decay`。
>
> 可以参见:
>
> 1、https://github.com/llvm-mirror/libcxx/blob/master/include/thread

```C++
template< class Function, class... Args >
explicit thread( Function&& f, Args&&... args );
```



3、[std::async](https://en.cppreference.com/w/cpp/thread/async)

```C++
template< class Function, class... Args>
std::future<std::result_of_t<std::decay_t<Function>(std::decay_t<Args>...)>>
    async( Function&& f, Args&&... args );
```



可以看到，它们的写法是比较类似的。

## cppreference [Template parameters and template arguments](https://en.cppreference.com/w/cpp/language/template_parameters)

> NOTE: 其中对function as template argument进行了说明，其实就是转换为function pointer。



## stackoverflow [Function passed as template argument](https://stackoverflow.com/questions/1174169/function-passed-as-template-argument)

> NOTE: 
>
> 1、`add1`、`add2` 是function pointer
>
> 2、`add3`是functor
>
> 能否实现: 一个template，能够接受上述三者为template argument？这是提问者的问题；

I'm looking for the rules involving passing C++ templates functions as arguments.

This is supported by C++ as shown by an example here:

```cpp
#include <iostream>

void add1(int &v)
{
  v+=1;
}

void add2(int &v)
{
  v+=2;
}

template <void (*T)(int &)>
void doOperation()
{
  int temp=0;
  T(temp);
  std::cout << "Result is " << temp << std::endl;
}

int main()
{
  doOperation<add1>();
  doOperation<add2>();
}
```

> NOTE: template parameter是function pointer

Also, is there a way to allow a functor with the same signature to be used interchangeably with explicit functions during this kind of template invocation?

> NOTE: 这是提问者的问题；

The following does **not** work in the above program, at least in [Visual C++](http://en.wikipedia.org/wiki/Visual_C%2B%2B#32-bit_versions), because the syntax is obviously wrong. It'd be nice to be able to switch out a function for a functor and vice versa, similar to the way you can pass a function pointer or functor to the `std::sort` algorithm if you want to define a custom comparison operation.

```cpp
   struct add3 {
      void operator() (int &v) {v+=3;}
   };
...

    doOperation<add3>();
```

### [A](https://stackoverflow.com/a/1174193)

> NOTE: 这个回答给出了能够接受function pointer、functor的写法；

Yes, it is valid.

As for making it work with functors as well, the usual solution is something like this instead:

```cpp
template <typename F>
void doOperation(F f)
{
  int temp=0;
  f(temp);
  std::cout << "Result is " << temp << std::endl;
}
```

which can now be called as either:

```cpp
doOperation(add2);
doOperation(add3());
```

[See it live](https://godbolt.org/z/3pwX4G)

The problem with this is that if it makes it tricky for the compiler to inline the call to `add2`, since all the compiler knows is that a function pointer type `void (*)(int &)` is being passed to `doOperation`. (But `add3`, being a functor, can be inlined easily. Here, the compiler knows that an object of type `add3` is passed to the function, which means that the function to call is `add3::operator()`, and not just some unknown function pointer.)

> NOTE: functor VS function pointer，在下面的**Comments**章节中，也对这个topic进行了讨论

#### Comments

There is an advantage to using functors over function pointers. The functor can be instanciated inside the class and thus provides more opertunity to the compiler for optimizations (such as inlining). The compiler would be hard pressed to optimize a call over a function pointer. – [Martin York](https://stackoverflow.com/users/14065/martin-york) [Jul 23 '09 at 20:41](https://stackoverflow.com/questions/1174169/function-passed-as-template-argument#comment994667_1174193)

#### Template parameters taking a non-type function pointer parameter 

When the function is used in a template parameter it 'decays' into a pointer to the passed function. It's analagous to how arrays decay into pointers when passed as arguments to parameters. Of course, the pointer value is known at compile time and must point to a function with external linkage so that compiler can use this information for optimization purposes. – [CB Bailey](https://stackoverflow.com/users/19563/cb-bailey) [Jul 23 '09 at 20:43](https://stackoverflow.com/questions/1174169/function-passed-as-template-argument#comment994679_1174193)

My comment above refers to template parameters taking a non-type function pointer parameter. In this case the template instantiation can be inlined as the compiler knows the value of the pointer at compile time. In jalf's example the template takes a type parameter and the type of the template parameter and the value of the function argument together determine the function called, a function object can be better optimized. – [CB Bailey](https://stackoverflow.com/users/19563/cb-bailey) [Jul 23 '09 at 20:46](https://stackoverflow.com/questions/1174169/function-passed-as-template-argument#comment994693_1174193)

> NOTE: 上面描述的decay是非常重要的









## Application

### Functional programming

#### STL algorithm

STL algorithm充分运用了function as template parameter，下面是非常好的例子:

1、`Library\Standard-library\STL\Containers-library\Associative-container\Map\Operations\Values-like-python-dict`

其中给出了非常好的例子；



### AOP

其实，感觉此处的AOP和functional programming有些类似；