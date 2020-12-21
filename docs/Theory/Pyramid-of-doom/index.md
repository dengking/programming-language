# Pyramid of doom 

是在阅读 benestudio [Async/await vs Coroutines vs Promises vs Callbacks](https://blog.benestudio.co/async-await-vs-coroutines-vs-promises-eaedee4e0829) 时，发现的这个术语。

## wikipedia [Pyramid of doom (programming)](https://en.wikipedia.org/wiki/Pyramid_of_doom_(programming))

> NOTE: 嵌套太深，导致需要编写很多重复的、冗杂代码

### Problem

```python
theWidth = windows("Main").views(5).size().width();
```

> NOTE: 这种写法存在很多问题，下面是安全，但是冗杂的方法

```C++
if windows.contains("Main") {
    if windows("Main").views.contains(5) {
        theWidth = windows("Main").views(5).size().width();
        //more code that works with theWidth
    }
}
```





### Solution

> NOTE: 总的来说，解决方案就是: Safe-navigation-operator，Apple's [Swift](https://en.wikipedia.org/wiki/Swift_(programming_language))  [optional chaining](https://en.wikipedia.org/wiki/Safe_navigation_operator) 、Microsoft's [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) 6.0 and [Visual Basic](https://en.wikipedia.org/wiki/Visual_Basic_.NET) 14  [null-conditional](https://en.wikipedia.org/wiki/Safe_navigation_operator) operators `?.` and `?[` 都属于这个范畴。

This sort of programming construct is very common and a number of programming languages have added some sort of [syntactic sugar](https://en.wikipedia.org/wiki/Syntactic_sugar) to address this. For instance, Apple's [Swift](https://en.wikipedia.org/wiki/Swift_(programming_language)) added the concept of [optional chaining](https://en.wikipedia.org/wiki/Safe_navigation_operator) in if statements[[5\]](https://en.wikipedia.org/wiki/Pyramid_of_doom_(programming)#cite_note-5) while Microsoft's [C#](https://en.wikipedia.org/wiki/C_Sharp_(programming_language)) 6.0 and [Visual Basic](https://en.wikipedia.org/wiki/Visual_Basic_.NET) 14 added the [null-conditional](https://en.wikipedia.org/wiki/Safe_navigation_operator) operators `?.` and `?[` for member access and indexing, respectively.[[6\]](https://en.wikipedia.org/wiki/Pyramid_of_doom_(programming)#cite_note-6)[[7\]](https://en.wikipedia.org/wiki/Pyramid_of_doom_(programming)#cite_note-7)[[8\]](https://en.wikipedia.org/wiki/Pyramid_of_doom_(programming)#cite_note-8) The basic idea is to allow a string of method calls to immediately return null if any of its members is null, so for instance:

```
theWidth = windows("Main")?.views(5)?.size.width;
```

would assign null to `theWidth` if either "Main" or the fifth subview is missing, or complete the statement and return the width if they are both valid. There are many times where the programmer wants to take different actions in these two cases, so Swift adds another form of syntactic sugar for this role, the `if let` statement, also known as "optional binding":

```swift
if let theView = windows("Main")?.views(5) {
    //do things knowing the view exists...
    theWidth = theView.size.width
}
```

#### See also

- [Promises](https://en.wikipedia.org/wiki/Futures_and_promises), a technique for avoiding the pyramid of doom, e.g. used in JavaScript[[9\]](https://en.wikipedia.org/wiki/Pyramid_of_doom_(programming)#cite_note-9)
- [Safe navigation operator](https://en.wikipedia.org/wiki/Safe_navigation_operator), a programming language operator that lets one avoid the pyramid of doom