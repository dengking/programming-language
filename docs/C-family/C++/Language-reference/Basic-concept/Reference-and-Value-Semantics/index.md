# 关于本章

在阅读zhihu [如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118) 时，其中的内容引起了我对C++ value semantic 和 reference semantic的思考，经过一番思考后，我发现，它们两者对于理解C++至关重要，需要进行专门的总结。



## zhihu [如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118)

[值语义](https://link.zhihu.com/?target=http%3A//www.parashift.com/c%2B%2B-faq/val-vs-ref-semantics.html)是很多OO语言（比如python、java）里没有的概念。在这些语言里，几乎所有的变量都是引用语义(**Reference Semantics**)，GC掌管了所有对象的生命期管理事务， 程序员无需为此操心，只需要用变量去对象池中去引用即可。值语义虽然也有出场的机会，例如Java里的[Primitive Data Type](https://link.zhihu.com/?target=http%3A//docs.oracle.com/javase/tutorial/java/nutsandbolts/datatypes.htmlPrimitive)，但毕竟不是重点，所以往往被忽视。

还有一个很重要的原因，在OO语言里，

> [值语义和运行时多态是矛盾的。](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2012/02/03/value-semantics/%23comment-270)

值语义因此也成为了C++区别于其它OO语言的特征之一。

所以，在不理解**值语义**的情况下去谈**右值引用**，最多也是浅尝辄止，所以"感觉奇怪"也是很正常的，甚至会出现类似[“C++的数组不支持多态”？](https://link.zhihu.com/?target=http%3A//coolshell.cn/articles/9543.html)这样的问题。

### 值语义的作用



首先给出一篇[blog](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2012/02/03/value-semantics/)作为值语义的科普，内容则不再复述，以下只讨论其意义。

这篇blog的作者[Andrzej Krzemieński](https://link.zhihu.com/?target=https%3A//github.com/akrzemi1)，曾在另一篇blog中盛赞RAII，并认为它是[C++'s best feature](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2013/07/18/cs-best-feature/)，[Herb Sutter](https://link.zhihu.com/?target=http%3A//herbsutter.com/about/)也在[文章的回复](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2013/07/18/cs-best-feature/%23comment-1424)中提到:

> RAII code is just naturally exception-safe with immediate cleanup, not leaking stuff until the GC gets around to finding it. This is especially important with scarce resources like file handles.



但问题是，像`C#`的[using statement](https://link.zhihu.com/?target=http%3A//msdn.microsoft.com/en-us/library/yh598w02%28v%3Dvs.80%29.aspx)和Java的[try-with-resources statement](https://link.zhihu.com/?target=http%3A//docs.oracle.com/javase/tutorial/essential/exceptions/tryResourceClose.html)同样具有RAII的特点，但仍然有人会提出["RAII: Why is unique to C++?"](https://link.zhihu.com/?target=http%3A//swiftcoder.wordpress.com/2009/02/18/raii-why-is-it-unique-to-c/)这样的问题。原因即在于C++独有的值语义：**程序员通过**值语义可以方便直观地控制对象生命期（object lifetime），让RAII用起来更自然。

> NOTE: C++ programmer能够control object lifetime。

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

所以，**值语义**在C++里的作用之一就是用于控制对象生命期([另一个作用就是提升性能](https://link.zhihu.com/?target=http%3A//blog.csdn.net/Solstice/article/details/6692976))，以方便通过RAII写出简洁自然、异常安全的代码。该意义非常重大，这也是**右值引用**在[C++ - State of the Evolution](https://link.zhihu.com/?target=http%3A//www2.open-std.org/jtc1/sc22/wg21/docs/papers/2007/n2291.html)上稳坐头把交椅的原因。

### 右值引用与值语义


前面提到，**值语义**用于控制对象的生命期，而其具体的控制方式分为两种：

- 生命期限于scope内：无需控制，到期自动调用析构函数。
- 需要延长到scope外：移动语义。

因为右值引用的目的在于[实现移动语义](https://link.zhihu.com/?target=http%3A//www.open-std.org/jtc1/sc22/wg21/docs/papers/2006/n2027.html%23rvalue_reference)，所以**右值引用** 意义即是**加强了值语义对对象生命期的控制能力**。

在移动语义的作用下，[Effective C++](https://link.zhihu.com/?target=http%3A//book.douban.com/subject/1231590/) 条款23从此作古：

> 当你必须传回object时，不要尝试传回reference。


资源管理类，如[std::ifstream](https://link.zhihu.com/?target=http%3A//en.cppreference.com/w/cpp/io/basic_ifstream)、[boost::asio::basic_stream_socket](https://link.zhihu.com/?target=http%3A//www.boost.org/doc/libs/1_56_0/doc/html/boost_asio/reference/basic_stream_socket.html)，可以光明正大地当成`int`来用，而无需担心类似[Effective STL](https://link.zhihu.com/?target=http%3A//book.douban.com/subject/1792179/) 条款8那样的问题：

> 切勿创建包含`auto_ptr`的容器对象。



### 右值引用与完美转发



右值引用还有一个作用是实现完美转发。[完美转发可以在一定程度上让代码保持简洁](https://link.zhihu.com/?target=http%3A//www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm)，但同时，这也引入了一些[令人讨厌的坑](https://link.zhihu.com/?target=http%3A//scottmeyers.blogspot.com/2012/10/copying-constructors-in-c11.html)。个人感觉意义不如移动语义的重大，所以这里不再展开了。

\---------------
附参考资料

- [C++ future and the pointer](https://link.zhihu.com/?target=http%3A//www.meetingcpp.com/index.php/br/items/cpp-future-and-the-pointer.html). Jens Weller
- [Back to the Basics! Essentials of Modern C++ Style](https://link.zhihu.com/?target=https%3A//github.com/CppCon/CppCon2014/blob/master/Presentations/Back%20to%20the%20Basics!%20Essentials%20of%20Modern%20C%2B%2B%20Style/Back%20to%20the%20Basics!%20Essentials%20of%20Modern%20C%2B%2B%20Style%20-%20Herb%20Sutter%20-%20CppCon%202014.pdf%3Fraw%3Dtrue). Herb Sutter
- [Move Constructor](https://link.zhihu.com/?target=http%3A//akrzemi1.wordpress.com/2011/08/11/move-constructor/). Andrzej Krzemieński
- [A proposal to Add Move Semantics Support to C++ Language](https://link.zhihu.com/?target=http%3A//www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1377.htm). Howard E. Hinnant, Peter Dimov, Dave Abrahams
- [C++ 工程实践(8)：值语义](https://link.zhihu.com/?target=http%3A//www.cnblogs.com/solstice/archive/2011/08/16/2141515.html). 陈硕



## SUMMARY

zhihu [如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118)中的内容是非常精简但是内容丰富，需要对一些内容进行扩展，经过仔细梳理、思考发现，C++中的很多内容都与它有着密切的关联，下面对此进行梳理。

### Object

理解本节内容的一个非常重要的前提是: 理解C++ data model: object，参见 `C++\Language-reference\Basic-concept\Object` 章节。

### C++是兼容并包的

C++是兼容并包的语言，它既包含value semantic又包含reference semantic；



### C++对value semantic的增强

C++如下是对value semantic的增强:

1、copy elision

2、RVO

3、move semantic





### Value semantic is default

在c++中，value semantic is default，即默认是value semantic，如果要实现reference semantic，则需要使用`&`。而在java、python中则正好相反，reference semantic is default；

下面是akrzemi1 [Value semantics](https://akrzemi1.wordpress.com/2012/02/03/value-semantics/)中的例子

> In C++, `A = B` means that the value of `B` was assigned to `A`. In languages like C# and Java, `A = B` means that the reference `A` now points to whatever `B` was pointing to. Very different.



> In C++, you can instantiate instances (objects) of a Class (user defined type) with the normal value semantics of the built in types. For example,
>
> ```c++
> float a;
> MyClass b;
> ```
>
> We can also declare a pointer reference if we want:
>
> ```c++
> MyClass* c = new MyClass();
> ```
>
> In `C#`, we can only declare a reference:
>
> ```c++
> MyClass c = new MyClass();
> ```
>
> So, C# and Java have reduced functionality compared to C++.



### Runtime polymorphism

C++中需要通过reference semantic才能够实现runtime polymorphism，关于此，在`C++\Language-reference\Classes\Subtype-polymorphism`章节进行了讨论。



### Value category

这在`C++\Language-reference\Expressions\Value-categories`章节进行了讨论。

### 性能

value semantic和reference semantic和program的性能是存在一定关联的，这其中有着复杂的原因，其中一个非常重要的原因是: Compiler optimization，这在`C++\Language-reference\Basic-concept\Reference-and-Value-Semantics\Value-and-reference-semantics.md`中进行了深入讨论。





### TO READ

https://stackoverflow.com/questions/3106110/what-is-move-semantics/3109981#3109981



https://www.zhihu.com/question/22111546



https://isocpp.org/wiki/faq/value-vs-ref-semantics#virt-data



http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm



https://herbsutter.com/2014/05/21/cppcon-my-proposed-talks-part-2/



http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1385.htm



https://marcbeauchesne.com/return-value-optimization-copy-elision/



https://www.fluentcpp.com/2016/11/28/return-value-optimizations/



https://stackoverflow.com/questions/18335861/why-is-enum-class-preferred-over-plain-enum



https://stackoverflow.com/questions/11711034/stdshared-ptr-of-this

https://stackoverflow.com/questions/142391/getting-a-boostshared-ptr-for-this



