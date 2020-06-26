# Nifty Counter



## More C++ Idioms/[Nifty Counter](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Nifty_Counter)

### Intent

Ensure a non-local static object is initialized before its first use and destroyed only after last use of the object.

### Motivation

When **static objects** use other **static objects**, the initialization problem becomes more complex. A static object must be initialized before its use if it has **non-trivial initialization**. Initialization order of static objects across compilation units is not well-defined. Multiple static objects, spread across multiple **compilation units**, might be using a single static object. Therefore, it must be initialized before use. One example is `std::cout`, which is typically used by a number of other static objects.

### Solution and Sample Code

The "nifty counter" or "Schwarz counter" idiom is an example of a [reference counting idiom](https://en.wikibooks.org/w/index.php?title=More_C%2B%2B_Idiom/Reference_Counting&action=edit&redlink=1) applied to the initialization of static objects.

```c++
// Stream.h
#ifndef STREAM_H
#define STREAM_H

struct Stream {
  Stream ();
  ~Stream ();
};
extern Stream& stream; // global stream object

static struct StreamInitializer {
  StreamInitializer ();
  ~StreamInitializer ();
} streamInitializer; // static initializer for every translation unit

#endif // STREAM_H
```

```c++
// Stream.cpp
#include "Stream.h"

#include <new>         // placement new
#include <type_traits> // aligned_storage

static int nifty_counter; // zero initialized at load time
static typename std::aligned_storage<sizeof (Stream), alignof (Stream)>::type
  stream_buf; // memory for the stream object
Stream& stream = reinterpret_cast<Stream&> (stream_buf);

Stream::Stream ()
{
  // initialize things
}
Stream::~Stream ()
{
  // clean-up
} 

StreamInitializer::StreamInitializer ()
{
  if (nifty_counter++ == 0) new (&stream) Stream (); // placement new
}
StreamInitializer::~StreamInitializer ()
{
  if (--nifty_counter == 0) (&stream)->~Stream ();
}
```



The header file of the `Stream` class must be included before any member function can be called on the Stream object. An instance of the `StreamInitializer` class is included in each compilation unit. Any use of the `Stream` object follows the inclusion of the header, which ensures that the constructor of the initializer object is called before the Stream object is used.

The Stream class' header file declares a reference to the Stream object. In addition this reference is extern, meaning it is defined in one translation unit and accesses to it are resolved by the linker rather than the compiler.

The implementation file for the Stream class finally defines the Stream object, but in an unusual way: it first defines a static (i.e. local to the translation unit) buffer. This buffer is both, properly aligned and big enough, to store an object of type `Stream`. The reference to the Stream object defined in the header is then set to point to this buffer.

This buffer workaround enables fine-grained control of when the Stream object's constructor and destructor are called. In the example above, the constructor is called within the constructor of the first `StreamInitializer` object - using **placement new** to place it within the buffer. The Stream object's destructor is called, when the last `StreamInitializer` is destroyed,

This workaround is necessary because defining a Stream variable within `Stream.cpp` - be it static or not - will define it after the `StreamInitializer`, which is defined by including the header. Then, the `StreamInitializer`'s constructor is run before Stream's constructor and even worse, the initializer's destructor is run after the Stream object's destructor. The buffer solution above avoids this.



### Known Uses

Standard C++ iostream library `std::cout`, `std::cin`, `std::cerr`, `std::clog`.

### Related Idioms

[Reference Counting](https://en.wikibooks.org/w/index.php?title=More_C%2B%2B_Idioms/Reference_Counting&action=edit&redlink=1)