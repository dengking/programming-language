# Avoid singletons

一、需要注意的是: "One size does not fit all tradeoff"，有些情况下，是可以使用的，关于这样的情况:

1、参见 stackoverflow [What is so bad about singletons? [closed]](https://stackoverflow.com/questions/137975/what-is-so-bad-about-singletons) # [A](https://stackoverflow.com/a/142450)

2、singleton registry

## CppCoreGuidelines I.3: Avoid singletons

## Reason

Singletons are basically complicated global objects in disguise.

> NOTE: 本质上就是global object

### Exception

You can use the simplest "singleton" (so simple that it is often not considered a singleton) to get **initialization on first use**, if any:



```C++
X& myX()
{
    static X my_x {3};
    return my_x;
}
```

> NOTE: singleton function

Note that the initialization of a **local static** does not imply a **race condition**.



However, if the destruction of `X` involves an operation that needs to be synchronized we must use a less simple solution.

```C++
X& myX()
{
    static auto p = new X {3};
    return *p; // potential leak
}
```

Now someone must `delete` that object in some suitably thread-safe way.

That's error-prone, so we don't use that technique unless

- `myX` is in multithreaded code,
- that `X` object needs to be destroyed (e.g., because it releases a resource), and
- X 's destructor's code needs to be synchronized.

If you, as many do, define a singleton as a class for which only one object is created, functions like  `myX` are not singletons, and this useful technique is not an exception to the no-singleton rule.

## stackoverflow [What is so bad about singletons? [closed]](https://stackoverflow.com/questions/137975/what-is-so-bad-about-singletons)

### [A](https://stackoverflow.com/a/138012)

Paraphrased from Brian Button:

1、They are generally used as a **global instance**, why is that so bad? Because you hide the dependencies of your application in your code, instead of exposing them through the **interfaces**. Making something global to avoid passing it around is a [code smell](https://en.wikipedia.org/wiki/Code_smell).

> NOTE: 
>
> 1、"Making something global to avoid passing it around"是很多人的做法，显然这种做法是错误的
>
> 2、上面这段话给出了正确的做法

2、They violate the [single responsibility principle](https://en.wikipedia.org/wiki/Single_responsibility_principle): by virtue of the fact that they control their own creation and lifecycle.

3、They inherently cause code to be tightly [coupled](https://en.wikipedia.org/wiki/Coupling_(computer_programming)). This makes faking them out under test rather difficult in many cases.



4、They carry state around for the lifetime of the application. Another hit to testing since you can end up with a situation where tests need to be ordered which is a big no no for unit tests. Why? Because each unit test should be independent from the other.



### [A](https://stackoverflow.com/a/142450)

Singletons solve one (and only one) problem.

**Resource Contention.**

If you have some resource that

(**1**) can only have a single instance, and

(**2**) you need to manage that single instance,

you need a **singleton**.

There aren't many examples. A log file is the big one. You don't want to just abandon a single log file. You want to flush, sync and close it properly. This is an example of a single shared resource that has to be managed.

It's rare that you need a singleton. The reason they're bad is that they feel like a [global](https://en.wikipedia.org/wiki/Global_variable) and they're a fully paid up member of the GoF *[Design Patterns](https://en.wikipedia.org/wiki/Design_Patterns)* book.

When you think you need a global, you're probably making a terrible design mistake.

## Singletons make it hard to test your code

参见 :

1、codereview.stackexchange [Modern C++ Singleton Template](https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template) # [A](https://codereview.stackexchange.com/a/173942)