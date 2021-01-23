# C++ Core Guidelines: Passing Smart Pointers



## modernescpp [C++ Core Guidelines: Passing Smart Pointers](https://www.modernescpp.com/index.php/c-core-guidelines-passing-smart-pointer)

### [R.32: Take a `unique_ptr` parameter to express that a function assumes ownership of a `widget`](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-uniqueptrparam)

If a function should take ownership of a `Widget`, you should take the `std::unique_ptr<Widget>` by **copy**. The consequence is that the caller has to move the `std::unique_ptr<Widget>` to make the code to run.

> NOTE: 
>
> 1、[std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr) 是 "satisfies the requirements of [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) and [*MoveAssignable*](https://en.cppreference.com/w/cpp/named_req/MoveAssignable), but not the requirements of either [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) or [*CopyAssignable*](https://en.cppreference.com/w/cpp/named_req/CopyAssignable)"，而下面的 `sink(std::unique_ptr<Widget> uniqPtr)` 的入参是 pass-by-value，因此，它只能够使用  [*MoveConstructible*](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) ，而不能够使用  [*CopyConstructible*](https://en.cppreference.com/w/cpp/named_req/CopyConstructible) ，因此，它就需要它的调用者，在调用的时候: `std::move(uniqPtr)`，其实它的这个用法和我们使用`std::string` **pass-by-value**是一样的
>
> 2、下面的idiom让我想起了resource return idiom

```c++
#include <memory>
#include <utility>

struct Widget{
    Widget(int){}
};

void sink(std::unique_ptr<Widget> uniqPtr){
    // do something with uniqPtr
}

int main(){
    auto uniqPtr = std::make_unique<Widget>(1998);
    
    sink(std::move(uniqPtr));      // (1)
    sink(uniqPtr);                 // (2) ERROR
}
```

> NOTE: 上述代码需要C++14，下面是可以坐在C++11中编译通过的代码

```c++
#include <memory>
#include <utility>

struct Widget{
    Widget(int){}
};

void sink(std::unique_ptr<Widget> uniqPtr){
    // do something with uniqPtr
}

int main(){
    std::unique_ptr<Widget> uniqPtr {new Widget(1998)};
    
    sink(std::move(uniqPtr));      // (1)
    // sink(uniqPtr);                 // (2) ERROR
}
```



> NOTE:
>
> ```C++
> test.cpp: 在函数‘int main()’中:
> test.cpp:16:17: 错误：使用了被删除的函数‘std::unique_ptr<_Tp, _Dp>::unique_ptr(const std::unique_ptr<_Tp, _Dp>&) [with _Tp = Widget; _Dp = std::default_delete<Widget>]’
>      sink(uniqPtr);                 // (2) ERROR
>                  ^
> In file included from /usr/include/c++/4.8.2/memory:81:0,
>                  from test.cpp:1:
> /usr/include/c++/4.8.2/bits/unique_ptr.h:273:7: 错误：在此声明
>        unique_ptr(const unique_ptr&) = delete;
>        ^
> test.cpp:8:6: 错误：  初始化‘void sink(std::unique_ptr<Widget>)’的实参 1
>  void sink(std::unique_ptr<Widget> uniqPtr){
> 
> ```



The call (1) is fine but the call (2) breaks because you can not copy an `std::unique_ptr`. If your function only wants to use the `Widget`, it should take its parameter by pointer of by reference. The difference between a pointer and a reference is that a pointer can be a null pointer.

```c++
void useWidget(Widget* wid);
void useWidget(Widget& wid);
```



### [R.33: Take a `unique_ptr&` parameter to express that a function reseats the `widget`](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-reseat)

Sometimes a function want's to reseat a Widget. In this use-case, you should pass the `std::unique_ptr<Widget>` by a non-const reference.

> NOTE: reseat 的含义是`reset`

```C++
#include <memory>
#include <utility>

struct Widget{
    Widget(int){}
};

void reseat(std::unique_ptr<Widget>& uniqPtr){
    uniqPtr.reset(new Widget(2003));   // (0)
    // do something with uniqPtr
}

int main(){
    auto uniqPtr = std::make_unique<Widget>(1998);
    
    reseat(std::move(uniqPtr));       // (1) ERROR
    reseat(uniqPtr);                  // (2) 
}
```

 

Now, the call (1) fails because you can not bind an rvalue to a non-const lvalue reference. This will not hold for the copy in (2). A lvalue can be bound to an lvalue reference. By the way. The call (0) will not only construct a new `Widget(2003)`, it will also destruct the old `Widget(1998)`.

The next three rules to `std::shared_ptr` are literally repetitions; therefore, I will make one out of them.



### [R.34: Take a `shared_ptr` parameter to express that a function is part owner](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-sharedptrparam-owner)



### [R.35: Take a `shared_ptr&` parameter to express that a function might reseat the shared pointer](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-sharedptrparam)



### [R.36: Take a `const shared_ptr&` parameter to express that it might retain a reference count to the object ???](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-sharedptrparam-const)



Here are the three function signatures, we have to deal with.

```c++
void share(std::shared_ptr<Widget> shaWid);
void reseat(std::shard_ptr<Widget>& shadWid);
void mayShare(const std::shared_ptr<Widget>& shaWid);
```

 

Let's look at each function signature in isolation. What does this mean from the function perspective?

- **`void share(std::shared_ptr<Widget> shaWid)`**: I'm for the lifetime of the function body a shared owner of the `Widget`. At the begin of the function body, I will increase the reference counter; at the end of the function, I will decrease the reference counter; therefore, the Widget will stay alive, as long as I use it.
- **`void reseat(std::shared_ptr<Widget>& shaWid)`**: I'm not a shared owner of the Widget, because I will not change the **reference counter**. I have not guaranteed that the Widget will stay alive during the execution of my function, but I can reseat the resource. A non-const lvalue reference is more like: I borrow the resource and can reseat it. 
- **`void mayShare(const std::shared_ptr<Widget>& shaWid)`**: I only borrow the resource. Either can I extend the lifetime of the resource nor can I reseat the resource. To be honest, you should use a pointer (`Widget*`) or a reference (`Widget&`) as a parameter instead, because there is no added value in using a `std::shared_ptr`.



### [R.37: Do not pass a pointer or reference obtained from an aliased smart pointer](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-smartptrget)

Let me present you a short code snippet to make the rule clear.

```c++
void oldFunc(Widget* wid){
  // do something with wid
}

void shared(std::shared_ptr<Widget>& shaPtr){       // (2)
    
   oldFunc(*shaPtr);                                // (3)
   
   // do something with shaPtr
     
 }

auto globShared = std::make_shared<Widget>(2011);   // (1)


...

shared(globShared);                                 
```

 

`globShared` (1) is a globally shared pointer. The function `shared` takes it argument per reference (2). Therefore, the reference counter of `shaPtr` will no be increased and the function share will no extend the lifetime of `Widget(2011)`. The issue begins with (3). `oldFunc` accepts a pointer to the Widget; therefore, `oldFunc` has no guarantee that the Widget will stay alive during its execution. `oldFunc` only borrows the Widget.

> NOTE: “`oldFunc` has no guarantee that the Widget will stay alive during its execution”这是混用smart pointer和row pointer所带来的问题，在multiple thread中，可能存在这种情况。

The cure is quite simple. You have to ensure that the reference count of `globShared` will be increased before the call to the function `oldFunc`. This means you have to make a copy of the `std::shared_ptr`:

- Pass the `std::shared_ptr` by copy to the function shared:

  ```c++
  void shared(std::shared_ptr<Widget> shaPtr){
     
     oldFunc(*shaPtr);
     
     // do something with shaPtr
       
   } 
  ```

- Make a copy of the `shaPtr`  in the function shared:

  ```c++
   void shared(std::shared_ptr<Widget>& shaPtr){
     
     auto keepAlive = shaPtr;   
     oldFunc(*shaPtr);
     
     // do something with keepAlive or shaPtr
       
   } 
  ```

  > NOTE: 相比之下，这种写法较上一种更加意图明显。

The same reasoning also applies to `std::unique_ptr` but I have no simple cure in mind because you can not copy an `std::unique_ptr`. I suggest you should clone your `std::unique_ptr` and, therefore, make a new `std::unique_ptr`.