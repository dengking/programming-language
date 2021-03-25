# Base-from-Member



## More C++ Idioms/[Base-from-Member](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Base-from-Member)

> NOTE: 充分利用order of initialization即“**base classes** are initialized in the order they are declared”，引入一个class来初始化基类的constructor中需要使用的variable，并且保证这个类的次序在基类之前。

In C++, base classes are initialized before any member of the derived classes. The reason for this is that members of a **derived class** may use *base* part of the object. Therefore, all the *base* parts (i.e., all the base classes) must be initialized before members of the **derived class**. Sometimes, however, it becomes necessary to initialize a **base class** from a data member that is available only in the **derived class**. It sounds contradictory to the rules of C++ language because the parameter (a member of derived class) that is passed to the base class constructor must be **fully initialized**. This creates circular initialization problem (an [infinite regress](https://en.wikipedia.org/wiki/Infinite_regress)).

The following code, obtained from Boost[[1\]](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Base-from-Member#cite_note-1) library, shows the problem.

```c++
#include <streambuf>  // for std::streambuf
#include <ostream>    // for std::ostream

namespace  std {
  class streambuf;
  class ostream {
    explicit ostream(std::streambuf * buf);
    //...
  };
}
class fdoutbuf   // A customization of streambuf
    : public std::streambuf
{
public:
    explicit fdoutbuf( int fd );
    //...
};

class fdostream
    : public std::ostream
{
protected:
    fdoutbuf buf;
public:
    explicit fdostream( int fd ) 
        : buf( fd ), std::ostream( &buf ) 
        // This is not allowed: buf can't be initialized before std::ostream.
        // std::ostream needs a std::streambuf object defined inside fdoutbuf.
    {}
};

```

> NOTE: 下面是对上述程序的改写，以使它能够编译

```c++



class streambuf
{
    
};

class ostream {
    public:
    explicit ostream(streambuf * buf)
    {
    }
    //...
};

class fdoutbuf   // A customization of streambuf
    : public streambuf
{
public:
    explicit fdoutbuf( int fd )
    {
    }
    //...
};

class fdostream
    : public ostream
{
protected:
    fdoutbuf buf;
public:
    explicit fdostream( int fd ) 
        : buf( fd ), ostream( &buf ) 
        // This is not allowed: buf can't be initialized before std::ostream.
        // std::ostream needs a std::streambuf object defined inside fdoutbuf.
    {}
};


int main()
{
    fdostream f(1);
}
```

> NOTE: 
>
> 一、上述代码是能够编译通过的，`g++ test.cpp -fpermissive`
>
> 二、上述code的问题在于: 
>
> ```C++
> class fdostream
>  : public ostream
> {
> protected:
>  fdoutbuf buf;
> public:
>  explicit fdostream( int fd ) 
>      : buf( fd ), ostream( &buf ) 
>      // This is not allowed: buf can't be initialized before std::ostream.
>      // std::ostream needs a std::streambuf object defined inside fdoutbuf.
>  {}
> };
> ```
>
> `ostream( &buf )`是典型的使用子类的成员去初始化base class:
>
> 1、`fdostream`的initialization依赖于 base class `ostream`，而base class `ostream`的initialization又依赖于`fdostream`的member variable `buf`， 它就形成了circle dependency



The above code snippet shows a case where the programmer is interested in customizing the `std::streambuf` class. He/she does so in `fdoutbuf` by inheriting from `std::streambuf`. The `fdoutbuf` class is used as a member in `fdostream` class, which is-a kind of `std::ostream`. The `std::ostream` class, however, needs a pointer to a `std::streambuf` class, or its **derived class**. The type of pointer to `buf` is suitable but passing it makes sense only if `buf` is initialized. However, it won’t be initialized unless all base classes are initialized. Hence the **infinite regress**. The base-from-member idiom addresses this problem.

### Solution and Sample Code

This idiom makes use of the fact that **base classes** are initialized in the order they are declared. The **derived class** controls the order of its **base classes**, and in turn, controls the order in which they are initialized. In this idiom, a new class is added just to initialize the member in the **derived class** that is causing the problem. This new class is introduced in the **base-class-list** before all other **base classes**. Because the new class comes *before* the base class that needs the fully constructed parameter, it is initialized first and then the reference can be passed as usual. Here is the solution using **base-from-member idiom**.

```c++
#include <streambuf>  // for std::streambuf
#include <ostream>    // for std::ostream

class fdoutbuf
    : public std::streambuf
{
public:
    explicit fdoutbuf(int fd)
    {
    	//...        
    }

};

struct fdostream_pbase // A newly introduced class
{
    fdoutbuf sbuffer; // The member moved 'up' the hierarchy.
    explicit fdostream_pbase(int fd)
        : sbuffer(fd)
    {}
};

class fdostream
    : protected fdostream_pbase // This class will be initialized before the next one.
    , public std::ostream
{
public:
    explicit fdostream(int fd)
        : fdostream_pbase(fd),   // Initialize the newly added base before std::ostream.
          std::ostream(&sbuffer) //  Now safe to pass the pointer
    {}
    //...
};
int main(void)
{
  fdostream standard_out(1);
  standard_out << "Hello, World\n";

  return 0;
}
```

The `fdostream_pbase` class is the newly introduced class that now has the `sbuffer` member. The `fdostream` class inherits from this new class and adds it before `std::ostream` in its base class list. This ensures that `sbuffer` is initialized before and the pointer can be passed safely to the `std::ostream` constructor.

### Known Uses

- [Boost Base from Member](http://www.boost.org/doc/libs/1_47_0/libs/utility/base_from_member.html)

### References

- Boost Utility http://www.boost.org/doc/libs/1_61_0/libs/utility/doc/html/base_from_member.html