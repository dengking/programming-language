# zhihu Rvalue reference



## [如何评价 C++11 的右值引用（Rvalue reference）特性？ - Tinro的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/30801982) 

**右值引用**是C++11中最重要的新特性之一，它解决了C++中大量的历史遗留问题，使C++标准库的实现在多种场景下消除了不必要的额外开销（如[std::vector](https://en.cppreference.com/w/cpp/container/vector), [std::string](https://en.cppreference.com/w/cpp/string/basic_string))，也使得另外一些标准库（如[std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr), [std::function](https://en.cppreference.com/w/cpp/utility/functional/function)）成为可能。即使你并不直接使用右值引用，也可以通过标准库，间接从这一新特性中受益。为了更好的理解**标准库**结合**右值引用**带来的优化，我们有必要了解一下**右值引用**的重大意义。

右值引用的意义通常解释为两大作用：移动语义和完美转发。本文主要讨论移动语义。

移动语义
------

移动语义，简单来说解决的是各种情形下**对象的资源所有权转移**的问题。而在C++11之前，**移动语义**的缺失是C++饱受诟病的问题之一。

举个栗子。

问题一：如何将大象放入冰箱？
答案是众所周知的。首先你需要有一台特殊的冰箱，这台冰箱是为了装下大象而制造的。你打开冰箱门，将大象放入冰箱，然后关上冰箱门。

问题二：如何将大象从一台冰箱转移到另一台冰箱？
普通解答：打开冰箱门，取出大象，关上冰箱门，打开另一台冰箱门，放进大象，关上冰箱门。

2B解答：在第二个冰箱中启动量子复制系统，克隆一只完全相同的大象，然后启动高能激光将第一个冰箱内的大象气化消失。

等等，这个2B解答听起来很耳熟，这不就是C++中要**移动**一个对象时所做的事情吗？

“移动”，这是一个三岁小孩都明白的概念。将大象（资源）从一台冰箱（对象）**移动**到另一台冰箱，这个行为是如此自然，没有任何人会采用先复制大象，再销毁大象这样匪夷所思的方法（不幸的是，过去c++就是采用的这种方式）。C++通过**拷贝构造函数**和**拷贝赋值操作符**为**类**设计了**拷贝/复制**的概念，但为了实现对资源的**移动**操作，调用者必须使用先复制、再析构的方式。否则，就需要自己实现移动资源的接口。

为了实现**移动语义**，首先需要解决的问题是，如何标识对象的资源是可以被移动的呢？这种机制必须以一种**最低开销**的方式实现，并且对所有的类都有效。C++的设计者们注意到，大多数情况下，**右值**所包含的对象都是可以**安全的被移动**的。

**右值**（相对应的还有**左值**）是从C语言设计时就有的概念，但因为其如此基础，也是一个最常被忽略的概念。不严格的来说，左值对应变量的存储位置，而右值对应变量的值本身。C++中**右值**可以被赋值给**左值**或者绑定到引用。类的右值是一个**临时对象**，如果没有被**绑定**到**引用**，在表达式结束时就会被废弃。于是我们可以在右值被废弃之前，移走它的资源进行废物利用，从而避免无意义的复制。被移走资源的右值在废弃时已经成为空壳，析构的开销也会降低。

**右值中的数据可以被安全移走**这一特性使得**右值**被用来表达**移动语义**。以同类型的右值构造对象时，需要以**引用形式**传入参数。**右值引用**顾名思义专门用来引用右值，**左值引用**和**右值引用**可以被分别重载，这样确保**左值**和**右值**分别调用到**拷贝**和**移动**的两种语义实现。对于**左值**，如果我们明确放弃对其资源的所有权，则可以通过[std::move()](https://en.cppreference.com/w/cpp/utility/move)来将其转为**右值引用**。[std::move()](https://en.cppreference.com/w/cpp/utility/move)实际上是`static_cast<T&&>()`的简单封装。

**右值引用**至少可以解决以下场景中的**移动语义**缺失问题：

### 按值传入参数

```c++
#include<string> // string
#include<utility> // move header
using namespace std;
class People {
public:
  People(string name) // 按值传入字符串，可接收左值、右值。接收左值时为复制，接收右值时为移动
  : name_(move(name)) // 显式移动构造，将传入的字符串移入成员变量
  {
  }
  string name_;
};

int main()
{
People a("Alice"); // 移动构造name

string bn = "Bob";
People b(bn); // 拷贝构造name    
}

```

构造`a`时，调用了一次字符串的**构造函数**和一次字符串的**移动构造函数**。如果使用`const string& name`接收参数，那么会有一次**构造函数**和一次**拷贝构造**，以及一次non-trivial的析构。尽管看起来很蛋疼，尽管编译器还有优化，但从语义来说按值传入参数是最优的方式。

> NOTE: 上面这段话中的“non-trivial的析构”所指为何？

> NOTE: 使用`const string& name`的写法如下：
>
> ```c++
> #include<string> // string
> 
> using namespace std;
> class People {
> public:
>   People(const string& name) 
>   : name_(name) 
>   {
>   }
>   string name_;
> };
> 
> int main()
> {
> People a("Alice"); // 拷贝构造name
> 
> string bn = "Bob";
> People b(bn); // 拷贝构造name    
> }
> 
> ```
>
> 上述两种写法都支持，programmer到底应该选择哪一种呢？

如果你要在构造函数中接收`std::shared_ptr<X>`并且存入类的成员（这是非常常见的），那么按值传入更是不二选择。拷贝`std::shared_ptr<X>`需要线程同步，相比之下移动`std::shared_ptr`是非常轻松愉快的。

> NOTE: 没有读懂

### 按值返回

```c++
void str_split(const string& s, vector<string>* vec); // 一个按值语义定义的字符串拆分函数。这里不考虑分隔符，假定分隔符是固定的。
```

这样要求`vec`在外部被事先构造，此时尚无从得知`vec`的大小。即使函数内部有办法预测`vec`的大小，因为函数并不负责构造`vec`，很可能仍需要`resize`。

对这样的函数嵌套调用更是痛苦的事情，谁用谁知道啊。

```c++
vector<string> str_split(const string& s) {
  vector<string> v;
  // ...
  return v; // v是左值，但优先移动，不支持移动时仍可复制。
}
```

如果函数按值返回，`return`语句又直接返回了一个栈上的左值对象（输入参数除外）时，标准要求优先调用**移动构造函数**，如果不符再调用**拷贝构造函数**。尽管`v`是左值，仍然会优先采用**移动语义**，返回`vector<string>`从此变得云淡风轻。此外，无论移动或是拷贝，可能的情况下仍然适用编译器优化，但语义不受影响。

> NOTE: 现代compiler基本上都支持RVO，关于RVO，参见`C++\Language-reference\Initialization\Copy-elision\Copy-elision.md`。

```c++
unique_ptr<SomeObj> create_obj(/*...*/) {
  unique_ptr<SomeObj> ptr(new SomeObj(/*...*/));
  ptr->foo(); // 一些可能的初始化
  return ptr;
}

```

```c++
unique_ptr<SomeObj> create_obj(/*...*/) {
  return unique_ptr<SomeObj>(new SomeObj(/*...*/));
}

```

在**工厂类**中，这样的语义是非常常见的。返回`unique_ptr`能够明确对所构造对象的所有权转移，特别的，这样的工厂类返回值可以被忽略而不会造成内存泄露。上面两种形式分别返回栈上的左值和右值，但都适用移动语义（`unique_ptr`不支持拷贝）。

> NOTE: 上述就是Resource Return Idiom，参见`C++\Idiom\OOP\Resource-Return\Resource-Return.md`

### 接收**右值表达式**

```c++
vector<string> str_split(const string& s);

vector<string> v = str_split("1,2,3"); // 返回的vector用以拷贝构造对象v。为v申请堆内存，复制数据，然后析构临时对象（释放堆内存）。
vector<string> v2;
v2 = str_split("1,2,3"); // 返回的vector被复制给对象v（拷贝赋值操作符）。需要先清理v2中原有数据，将临时对象中的数据复制给v2，然后析构临时对象。
```

注：`v`的拷贝构造调用有可能被优化掉，尽管如此在**语义上**仍然是有一次拷贝操作。

同样的代码，在支持移动语义的世界里就变得更美好了。

```c++
vector<string> str_split(const string& s);

vector<string> v = str_split("1,2,3"); // 返回的vector用以移动构造对象v。v直接取走临时对象的堆上内存，无需新申请。之后临时对象成为空壳，不再拥有任何资源，析构时也无需释放堆内存。
vector<string> v2;
v2 = str_split("1,2,3"); // 返回的vector被移动给对象v（移动赋值操作符）。先释放v2原有数据，然后直接从返回值中取走数据，然后返回值被析构。
```

注：v的移动构造调用有可能被优化掉，尽管如此在**语义上**仍然是有一次移动操作。

不用多说也知道上面的形式是多么常用和自然。而且这里完全没有任何对**右值引用**的**显式**使用，性能提升却默默的实现了。

### 对象存入容器

这个问题和前面的构造函数传参是类似的。不同的是这里是按两种引用分别传参。参见`std::vector`的[`push_back`](https://en.cppreference.com/w/cpp/container/vector/push_back) 函数。

```c++
void push_back( const T& value ); // (1)
void push_back( T&& value ); // (2)
```

不用多说自然是左值调用1右值调用2。如果你要往容器内放入超大对象，那么版本2自然是不2选择。

```c++
vector<vector<string>> vv;

vector<string> v = {"123", "456"};
v.push_back("789"); // 临时构造的string类型右值被移动进容器v
vv.push_back(move(v)); // 显式将v移动进vv
```

困扰多年的难言之隐是不是一洗了之了？

### `std::vector`的增长

又一个隐蔽的优化。当vector的存储容量需要增长时，通常会重新申请一块内存，并把原来的内容一个个复制过去并删除。对，复制并删除，改用移动就够了。

对于像`vector<string>`这样的容器，如果频繁插入造成存储容量不可避免的增长时，移动语义可以带来悄无声息而且美好的优化。

### `std::unique_ptr`放入容器

曾经，由于vector增长时会复制对象，像`std::unique_ptr`这样不可复制的对象是无法放入容器的。但实际上vector并不复制对象，而只是“移动”对象。所以随着移动语义的引入，`std::unique_ptr`放入`std::vector`成为理所当然的事情。

容器中存储`std::unique_ptr`有太多好处。想必每个人都写过这样的代码：

```c++
MyObj::MyObj() {
  for (...) {
    vec.push_back(new T());
  }
  // ...
}

MyObj::~MyObj() {
  for (vector<T*>::iterator iter = vec.begin(); iter != vec.end(); ++iter) {
    if (*iter) delete *iter;
  }
  // ...
}
```

繁琐暂且不说，异常安全也是大问题。使用`vector<unique_ptr<T>>`，完全无需显式析构，`unqiue_ptr`自会打理一切。完全不用写析构函数的感觉，你造吗？

`unique_ptr`是非常轻量的封装，存储空间等价于**裸指针**，但安全性强了一个世纪。实际中需要共享所有权的对象（指针）是比较少的，但需要转移所有权是非常常见的情况。`auto_ptr`的失败就在于其转移所有权的繁琐操作。`unique_ptr`配合移动语义即可轻松解决所有权传递的问题。

注：如果真的需要共享所有权，那么基于引用计数的`shared_ptr`是一个好的选择。`shared_ptr`同样可以移动。由于不需要线程同步，移动`shared_ptr`比复制更轻量。

### `std::thread`的传递

thread也是一种典型的不可复制的资源，但可以通过移动来传递所有权。同样[std::future](https://en.cppreference.com/w/cpp/thread/future)、 [std::promise](https://en.cppreference.com/w/cpp/thread/promise)、[std::packaged_task](https://en.cppreference.com/w/cpp/thread/packaged_task)等等这一票多线程类都是不可复制的，也都可以用移动的方式传递。

完美转发
------
除了移动语义，右值引用还解决了C++03中引用语法无法转发右值的问题，实现了完美转发，才使得`std::function`能有一个优雅的实现。这部分不再展开了。

总结
------

移动语义绝不是语法糖，而是带来了C++的深刻革新。移动语义不仅仅是针对库作者的，任何一个程序员都有必要去了解它。尽管你可能不会去主动为自己的类实现移动语义，但却时时刻刻都在享受移动语义带来的受益。因此这绝不意味着这是一个可有可无的东西。



## [如何评价 C++11 的右值引用（Rvalue reference）特性？ - zihuatanejo的回答 - 知乎](https://www.zhihu.com/question/22111546/answer/31929118) 

一句话答案：右值引用的出现是为了实现移动语义，顺便解决完美转发的问题，其意义在于扩充了值语义，帮助Modern C++可以全面地应用RAII。



