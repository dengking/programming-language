# Value and reference semantics

在阅读[如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118) 时，发现了其中对value semantic的讨论，其中“[值语义](https://link.zhihu.com/?target=http%3A//www.parashift.com/c%2B%2B-faq/val-vs-ref-semantics.html)”的链接的文章在下文中收录了。

## [如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118)

### 为什么会觉得右值引用很怪？

根本原因是**不理解值语义(Value Semantics)**。

[值语义](https://link.zhihu.com/?target=http%3A//www.parashift.com/c%2B%2B-faq/val-vs-ref-semantics.html)是很多OO语言里没有的概念。在这些语言里，几乎所有的变量都是引用语义(Reference Semantics)，GC掌管了所有对象的生命期管理事务， 程序员无需为此操心，只需要用变量去对象池中去引用即可。值语义虽然也有出场的机会，例如Java里的[Primitive Data Type](https://link.zhihu.com/?target=http%3A//docs.oracle.com/javase/tutorial/java/nutsandbolts/datatypes.htmlPrimitive)，但毕竟不是重点，所以往往被忽视。

还有一个很重要的原因，在OO语言里，

> [值语义和运行时多态是矛盾的。](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2012/02/03/value-semantics/%23comment-270)

值语义因此也成为了C++区别于其它OO语言的特征之一。

所以，在不理解值语义的情况下去谈右值引用，最多也是浅尝辄止，所以"感觉奇怪"也是很正常的，甚至会出现类似[“C++的数组不支持多态”？](https://link.zhihu.com/?target=http%3A//coolshell.cn/articles/9543.html)这样的问题。

### 值语义的作用

首先给出一篇[blog](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2012/02/03/value-semantics/)作为值语义的科普，内容则不再复述，以下只讨论其意义。

这篇blog的作者[Andrzej Krzemieński](https://link.zhihu.com/?target=https%3A//github.com/akrzemi1)，曾在另一篇blog中盛赞RAII，并认为它是[C++'s best feature](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2013/07/18/cs-best-feature/)，[Herb Sutter](https://link.zhihu.com/?target=http%3A//herbsutter.com/about/)也在[文章的回复](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2013/07/18/cs-best-feature/%23comment-1424)中提到:

> RAII code is just naturally exception-safe with immediate cleanup, not leaking stuff until the GC gets around to finding it. This is especially important with scarce resources like file handles.



但问题是，像C#的[using statement](https://link.zhihu.com/?target=http%3A//msdn.microsoft.com/en-us/library/yh598w02%28v%3Dvs.80%29.aspx)和Java的[try-with-resources statement](https://link.zhihu.com/?target=http%3A//docs.oracle.com/javase/tutorial/essential/exceptions/tryResourceClose.html)同样具有RAII的特点，但仍然有人会提出["RAII: Why is unique to C++?"](https://link.zhihu.com/?target=http%3A//swiftcoder.wordpress.com/2009/02/18/raii-why-is-it-unique-to-c/)这样的问题。原因即在于C++独有的**值语义**：**程序员通过值语义可以方便直观地控制对象生命期，让RAII用起来更自然。**

更何况像这段代码，

```cpp
vector<ifstream> produce()
{
  vector<ifstream> ans;
  for (int i = 0;  i < 10; ++i) {
    ans.emplace_back(name(i));
  } 
  return ans;
}
 
void consumer()
{
  vector<ifstream> files = produce();
  for (ifstream& f: files) {
    read(f);
  }
}
```

用try-with-resources根本就搞不定。当然，finally还是可以搞定，只是代码会很丑。

所以，**值语义**在C++里的作用之一就是用于控制对象生命期([另一个作用就是提升性能](https://link.zhihu.com/?target=http%3A//blog.csdn.net/Solstice/article/details/6692976))，以方便通过RAII写出简洁自然、异常安全的代码。该意义非常重大，这也是右值引用在[C++ - State of the Evolution](https://link.zhihu.com/?target=http%3A//www2.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2291.html)上稳坐头把交椅的原因。

### 右值引用与值语义


前面提到，值语义用于控制对象的生命期，而其具体的控制方式分为两种：

- - 生命期限于scope内：无需控制，到期自动调用析构函数。
  - 需要延长到scope外：移动语义。

因为右值引用的目的在于[实现移动语义](https://link.zhihu.com/?target=http%3A//www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2027.html%23rvalue_reference)，所以右值引用 意义即是**加强了值语义对对象生命期的控制能力**。

在移动语义的作用下，[Effective C++](https://link.zhihu.com/?target=http%3A//book.douban.com/subject/1231590/) 条款23从此作古：

> 当你必须传回object时，不要尝试传回reference。


资源管理类，如[std::ifstream](https://link.zhihu.com/?target=http%3A//en.cppreference.com/w/cpp/io/basic_ifstream)、[boost::asio::basic_stream_socket](https://link.zhihu.com/?target=http%3A//www.boost.org/doc/libs/1_56_0/doc/html/boost_asio/reference/basic_stream_socket.html)，可以光明正大地当成int来用，而无需担心类似[Effective STL](https://link.zhihu.com/?target=http%3A//book.douban.com/subject/1792179/) 条款8那样的问题：

> 切勿创建包含auto_ptr的容器对象。



### 右值引用与完美转发



右值引用还有一个作用是实现完美转发。[完美转发可以在一定程度上让代码保持简洁](https://link.zhihu.com/?target=http%3A//www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm)，但同时，这也引入了一些[令人讨厌的坑](https://link.zhihu.com/?target=http%3A//scottmeyers.blogspot.com/2012/10/copying-constructors-in-c11.html)。个人感觉意义不如移动语义的重大，所以这里不再展开了。

\---------------
附参考资料

- [C++ future and the pointer](https://link.zhihu.com/?target=http%3A//www.meetingcpp.com/index.php/br/items/cpp-future-and-the-pointer.html). Jens Weller
- [Back to the Basics! Essentials of Modern C++ Style](https://link.zhihu.com/?target=https%3A//github.com/CppCon/CppCon2014/blob/master/Presentations/Back%20to%20the%20Basics!%20Essentials%20of%20Modern%20C%2B%2B%20Style/Back%20to%20the%20Basics!%20Essentials%20of%20Modern%20C%2B%2B%20Style%20-%20Herb%20Sutter%20-%20CppCon%202014.pdf%3Fraw%3Dtrue). Herb Sutter
- [Move Constructor](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2011/08/11/move-constructor/). Andrzej Krzemieński
- [A proposal to Add Move Semantics Support to C++ Language](https://link.zhihu.com/?target=http%3A//www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1377.htm). Howard E. Hinnant, Peter Dimov, Dave Abrahams
- [C++ 工程实践(8)：值语义](https://link.zhihu.com/?target=http%3A//www.cnblogs.com/solstice/archive/2011/08/16/2141515.html). 陈硕





## Thoughts

在讨论value semantic的时候，我们会考虑到这个object的生命周期，而在reference semantic中，我们无需关注object的生命周期。

在value semantic中，我们copy一个object；在reference semantic中，我们reference一个object。

## Reference and Value Semantics [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

### What is value and/or reference semantics, and which is best in C++? [¶](https://isocpp.org/wiki/faq/value-vs-ref-semantics#val-vs-ref-semantics) [Δ](https://isocpp.org/wiki/faq/value-vs-ref-semantics#)

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