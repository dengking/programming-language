# Singleton

## 要求

为了确保只有一个object，需要满足如下要求:

1、只能够通过class static method `getInstance()`获得对象，其它方式都不可，包括:

a、`C c` 

因此，需要将constructor私有化。

2、不能够copy

## Meyer's singleton

Meyer's singleton就是利用static local object的特性来实现的，参见`Static-local-object`章节。

### stackoverflow [C++ Singleton design pattern](https://stackoverflow.com/questions/1008019/c-singleton-design-pattern)

[A](https://stackoverflow.com/a/1008289)

In 2008 I provided a C++98 implementation of the Singleton design pattern that is lazy-evaluated, guaranteed-destruction, not-technically-thread-safe:
[Can any one provide me a sample of Singleton in c++?](https://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104)

Here is an updated C++11 implementation of the Singleton design pattern that is lazy-evaluated, correctly-destroyed, and [thread-safe](https://stackoverflow.com/a/449823/52074).

```cpp
class S
{
    public:
        static S& getInstance()
        {
            static S    instance; // Guaranteed to be destroyed.
                                  // Instantiated on first use.
            return instance;
        }
    private:
        S() {}                    // Constructor? (the {} brackets) are needed here.

        // C++ 03
        // ========
        // Don't forget to declare these two. You want to make sure they
        // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
        // your singleton appearing.
        S(S const&);              // Don't Implement
        void operator=(S const&); // Don't implement

        // C++ 11
        // =======
        // We can use the better technique of deleting the methods
        // we don't want.
    public:
        S(S const&)               = delete;
        void operator=(S const&)  = delete;

        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
};
```

See this article about when to use a singleton: (not often)
[Singleton: How should it be used](https://stackoverflow.com/questions/86582/singleton-how-should-it-be-used)

See this two article about initialization order and how to cope:
[Static variables initialisation order](https://stackoverflow.com/questions/211237/c-static-variables-initialisation-order/211307#211307)
[Finding C++ static initialization order problems](https://stackoverflow.com/questions/335369/finding-c-static-initialization-order-problems/335746#335746)

See this article describing lifetimes:
[What is the lifetime of a static variable in a C++ function?](https://stackoverflow.com/questions/246564/what-is-the-lifetime-of-a-static-variable-in-a-c-function)

See this article that discusses some threading implications to singletons:
[Singleton instance declared as static variable of GetInstance method, is it thread-safe?](https://stackoverflow.com/questions/449436/singleton-instance-declared-as-static-variable-of-getinstance-method/449823#449823)

See this article that explains why double checked locking will not work on C++:
[What are all the common undefined behaviours that a C++ programmer should know about?](https://stackoverflow.com/questions/367633/what-are-all-the-common-undefined-behaviour-that-c-programmer-should-know-about/367690#367690)
[Dr Dobbs: C++ and The Perils of Double-Checked Locking: Part I](http://www.drdobbs.com/cpp/c-and-the-perils-of-double-checked-locki/184405726)

#### Thread safe

在 [C++11 introduced a standardized memory model. What does it mean? And how is it going to affect C++ programming?](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g) # [A](https://stackoverflow.com/a/6319356) 的如下comment中，对这个问题进行了讨论:

In most cases there is no sence to implement the double-checked locking initialization in C++11 since local static variable initialization is already thread-safe. – [newbie](https://stackoverflow.com/users/2489083/newbie) [Jun 17 '15 at 21:29](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment49844327_6319356)

@newbie: True, C+11 codified common practice and made the "Meyers singleton" thread-safe. I was just providing an example of the sort of thing you can do with the low-level synchronization primitives. – [Nemo](https://stackoverflow.com/users/768469/nemo) [Jun 17 '15 at 22:34](https://stackoverflow.com/questions/6319146/c11-introduced-a-standardized-memory-model-what-does-it-mean-and-how-is-it-g#comment49845972_6319356)

