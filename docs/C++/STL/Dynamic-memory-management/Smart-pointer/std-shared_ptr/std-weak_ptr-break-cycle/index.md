# `std::weak_ptr` break reference cycle

一、[`std::weak_ptr`](https://en.cppreference.com/w/cpp/memory/weak_ptr) 是 weak reference，主要用于break reference cycle。

二、素材:

1、stackoverflow [When is std::weak_ptr useful?](https://stackoverflow.com/questions/12030650/when-is-stdweak-ptr-useful)

2、nextptr [Using weak_ptr for circular references](https://www.nextptr.com/tutorial/ta1382183122/using-weak_ptr-for-circular-references) 

3、learncpp [M.8 — Circular dependency issues with std::shared_ptr, and std::weak_ptr](https://www.learncpp.com/cpp-tutorial/circular-dependency-issues-with-stdshared_ptr-and-stdweak_ptr/)





## cppreference [`std::weak_ptr`](https://en.cppreference.com/w/cpp/memory/weak_ptr) 

`std::weak_ptr` is a smart pointer that holds a non-owning ("weak") reference to an object that is managed by [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr). It must be converted to [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) in order to access the referenced object.

`std::weak_ptr` models temporary ownership: when an object needs to be accessed only if it exists, and it may be deleted at any time by someone else, `std::weak_ptr` is used to track the object, and it is converted to [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) to assume temporary ownership. If the original [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) is destroyed at this time, the object's lifetime is extended until the temporary [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr) is destroyed as well.

> NOTE: 
>
> Application一: track object lifetime: 
>
> 由`shared_ptr`来实现reference counting memory management(当reference count为0的时候，最后一个`shared_ptr` object会将pointee析构掉)，`std::weak_ptr` 可以用来判断pointee是否已经被释放，即track object lifetime，能够极好的解决dangling问题。
>
> 

Another use for `std::weak_ptr` is to break reference cycles formed by objects managed by [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr). If such cycle is orphaned (i.e., there are no outside shared pointers into the cycle), the `shared_ptr` reference counts cannot reach zero and the memory is leaked. To prevent this, one of the pointers in the cycle [can be made weak](https://en.cppreference.com/w/cpp/memory/weak_ptr/~weak_ptr#Example).

> NOTE:
>
> Application二: break reference cycle

### Example

Demonstrates how lock is used to ensure validity of the pointer.

> NOTE:
>
> 一、这个例子展示的是使用`std::weak_ptr` 来 "track object lifetime"

```C++
#include <iostream>
#include <memory>

std::weak_ptr<int> gw;

void observe()
{
    std::cout << "gw.use_count() == " << gw.use_count() << "; ";
    // we have to make a copy of shared pointer before usage:
    if (std::shared_ptr<int> spt = gw.lock())
    {
        std::cout << "*spt == " << *spt << '\n';
    }
    else
    {
        std::cout << "gw is expired\n";
    }
}

int main()
{
    {
        auto sp = std::make_shared<int>(42);
        gw = sp;

        observe();
    }

    observe();
}
```



## learncpp [M.8 — Circular dependency issues with std::shared_ptr, and std::weak_ptr](https://www.learncpp.com/cpp-tutorial/circular-dependency-issues-with-stdshared_ptr-and-stdweak_ptr/)



```c++
#include <iostream>
#include <memory> // for std::shared_ptr
#include <string>

class Person
{
    std::string m_name;
    std::shared_ptr<Person> m_partner; // initially created empty

public:
    Person(const std::string &name) : m_name(name)
    {
        std::cout << m_name << " created\n";
    }
    ~Person()
    {
        std::cout << m_name << " destroyed\n";
    }

    friend bool partnerUp(std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2)
    {
        if (!p1 || !p2)
            return false;

        p1->m_partner = p2;
        p2->m_partner = p1;

        std::cout << p1->m_name << " is now partnered with " << p2->m_name << '\n';

        return true;
    }
};

int main()
{
    auto lucy{std::make_shared<Person>("Lucy")};   // create a Person named "Lucy"
    auto ricky{std::make_shared<Person>("Ricky")}; // create a Person named "Ricky"

    partnerUp(lucy, ricky); // Make "Lucy" point to "Ricky" and vice-versa

    return 0;
}
```



> NOTE:
>
> 一、上述 `partnerUp` 函数的入参`std:shared_ptr` 是pass-by-reference
>
> 二、上述程序典型的展示了memory leak

### Circular references

#### A reductive case

```c++
#include <iostream>
#include <memory> // for std::shared_ptr

class Resource
{
public:
    std::shared_ptr<Resource> m_ptr{}; // initially created empty

    Resource() { std::cout << "Resource acquired\n"; }
    ~Resource() { std::cout << "Resource destroyed\n"; }
};

int main()
{
    auto ptr1{std::make_shared<Resource>()};

    ptr1->m_ptr = ptr1; // m_ptr is now sharing the Resource that contains it

    return 0;
}
```

> NOTE:
>
> 一、上述例子非常好，它是典型的一个节点包含指向自己的circle，它的正确写法如下:
>
> ```C++
> #include <iostream>
> #include <memory> // for std::shared_ptr and std::weak_ptr
> 
> class Resource
> {
> public:
>     std::weak_ptr<Resource> m_ptr{}; // use std::weak_ptr so m_ptr doesn't keep the Resource alive
> 
>     Resource() { std::cout << "Resource acquired\n"; }
>     ~Resource() { std::cout << "Resource destroyed\n"; }
> };
> 
> int main()
> {
>     auto ptr1{std::make_shared<Resource>()};
> 
>     ptr1->m_ptr = ptr1; // m_ptr is now sharing the Resource that contains it
> 
>     return 0;
> }
> ```
>
> 

### So what is std::weak_ptr for anyway?



```c++
#include <iostream>
#include <memory> // for std::shared_ptr and std::weak_ptr
#include <string>

class Person
{
    std::string m_name;
    std::weak_ptr<Person> m_partner; // note: This is now a std::weak_ptr

public:
    Person(const std::string &name) : m_name(name)
    {
        std::cout << m_name << " created\n";
    }
    ~Person()
    {
        std::cout << m_name << " destroyed\n";
    }

    friend bool partnerUp(std::shared_ptr<Person> &p1, std::shared_ptr<Person> &p2)
    {
        if (!p1 || !p2)
            return false;

        p1->m_partner = p2;
        p2->m_partner = p1;

        std::cout << p1->m_name << " is now partnered with " << p2->m_name << '\n';

        return true;
    }
};

int main()
{
    auto lucy{std::make_shared<Person>("Lucy")};
    auto ricky{std::make_shared<Person>("Ricky")};

    partnerUp(lucy, ricky);

    return 0;
}
```



### Dangling pointers with `std::weak_ptr`

```c++
// h/t to reader Waldo for this example
#include <iostream>
#include <memory>

class Resource
{
public:
    Resource() { std::cerr << "Resource acquired\n"; }
    ~Resource() { std::cerr << "Resource destroyed\n"; }
};

auto getWeakPtr()
{
    auto ptr{std::make_shared<Resource>()}; // Resource acquired

    return std::weak_ptr{ptr};
} // ptr goes out of scope, Resource destroyed

int main()
{
    std::cerr << "Getting weak_ptr...\n";

    auto ptr{getWeakPtr()}; // dangling

    std::cerr << "Done.\n";
}

```

