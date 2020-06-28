# Value and reference semantics

在阅读[如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118) 时，其中



在讨论value semantic的时候，我们会考虑到这个object的生命周期，而在reference semantic中，我们无需关注object的生命周期。



## What is value and/or reference semantics, and which is best in C++? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#val-vs-ref-semantics) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

With **reference semantics**, assignment is a pointer-copy (i.e., a *reference*). Value (or “copy”) semantics mean assignment copies the value, not just the pointer. C++ gives you the choice: use the assignment `operator` to copy the value (copy/value semantics), or use a pointer-copy to copy a pointer (reference semantics). C++ allows you to override the assignment `operator` to do anything your heart desires, however the default (and most common) choice is to copy the *value.*

> NOTE: 在java、python中，assignment的含义是bind；但是在c++中，assignment的含义是copy；使用本文的观点来看待这个问题就是：python、java都是reference semantic的，而c++是value semantic的。

> NOTE: C++默认是采用的value semantic，只有当我们显式地使用`*`来表示指针，使用`&`来表示reference。

Pros of reference semantics: flexibility and dynamic binding (you get **dynamic binding** in `C++` only when you pass by pointer or pass by reference, not when you pass by value).

> NOTE: C++中，只有当使用pointer、reference的时候，才能够实现dynamic binding。

Pros of value semantics: speed. “Speed” seems like an odd benefit for a feature that requires an object (vs. a pointer) to be copied, but the fact of the matter is that one usually accesses an object more than one copies the object, so the cost of the occasional copies is (usually) more than offset by the benefit of having an actual object rather than a pointer to an object.

> NOTE: 价值语义的优点:速度。
> “速度”似乎是一个奇怪的对一个功能需要一个对象(比一个指针)被复制,但事实是,通常一个对象访问对象的多个副本,所以偶尔的副本的成本(通常)超过抵消的好处有一个实际的对象而不是一个指向对象的指针。

There are three cases when you have an actual object as opposed to a pointer to an object: local objects, global/`static` objects, and fully contained member objects in a class. The most important of these is the last (“composition”).

More info about copy-vs-reference semantics is given in the next FAQs. Please read them all to get a balanced perspective. The first few have intentionally been slanted toward value semantics, so if you only read the first few of the following FAQs, you’ll get a warped perspective.

Assignment has other issues (e.g., shallow vs. deep copy) which are not covered here.





## Sounds like I should never use reference semantics, right? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#ref-semantics-sometimes-good) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)





## Does the poor performance of reference semantics mean I should pass-by-value? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#pass-by-value) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)