# Base-from-Member



## More C++ Idioms/[Base-from-Member](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Base-from-Member)

In C++, base classes are initialized before any member of the derived classes. The reason for this is that members of a **derived class** may use *base* part of the object. Therefore, all the *base* parts (i.e., all the base classes) must be initialized before members of the **derived class**. Sometimes, however, it becomes necessary to initialize a **base class** from a data member that is available only in the **derived class**. It sounds contradictory to the rules of C++ language because the parameter (a member of derived class) that is passed to the base class constructor must be fully initialized. This creates circular initialization problem (an [infinite regress](https://en.wikipedia.org/wiki/Infinite_regress)).

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

> NOTE: 上述代码是能够编译通过的