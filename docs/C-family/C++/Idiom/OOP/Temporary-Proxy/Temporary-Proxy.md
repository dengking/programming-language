# Temporary Proxy



## More C++ Idioms/[Temporary Proxy](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Temporary_Proxy)

> NOTE: 通过proxy class来添加一层indirection，在这层direction中，添加上一些特殊的operation。比如通过overload `operator []`和user-defined conversion来识别read、write，从而实现undo

### Motivation

The index operator (`operator []`) is often used to provide array like access syntax for user-defined classes. C++ standard library uses `operator []` in `std::string` and `std::map` classes. Standard string simply returns a character reference as a result of `operator []` whereas `std::map` returns a reference to the value given its key. In both cases, the returned reference can be directly read or written to. The string or map class have no knowledge or has no control over whether the reference is used for reading or for modification. Sometimes, however, it is useful to detect how the value is being used.

> NOTE: 我们只需要overload一些特殊的function，由compiler来负责调用这些function。在这些特殊的function中，添加上特殊的逻辑，这就是temporary proxy idiom的核心思想，在下面会对他进行专门介绍。

For example, consider an `UndoString` class, which supports a single *undo* operation in addition to the existing `std::string` interface. The design must allow an undo even though the character is accessed using the index operator. As mention above, `std::string` class has no knowledge of whether the result of `operator []` will be written to or not. The temporary proxy can be used to solve this problem.

### Solution and Sample Code

The temporary proxy idiom uses another object, conveniently called *proxy*, to detect whether the result of `operator []` is used for reading or writing. The `UndoString` class below defines its own non-const `operator []`, which takes place of `std::string`'s non-const `operator []`.



```c++
#include <iostream>
#include <string>
#include <stdexcept>
class UndoString : public std::string
{
  struct proxy
  {
    UndoString * str;
    size_t pos;

    proxy(UndoString * us, size_t position)
      : str(us), pos(position)
    {}

    // Invoked when proxy is used to modify the value.
    void operator = (const char & rhs) 
    {
      str->old = str->at(pos);
      str->old_pos = pos;
      str->at(pos) = rhs;
    }

    // Invoked when proxy is used to read the value.
    operator const char & () const
    {
      return str->at(pos);
    }
  };

  char old;
  int old_pos;

public:

  UndoString(const std::string & s)
    : std::string(s), old(0), old_pos(-1)
  {}

  // This operator replaces std::string's non-const operator [].
  proxy operator [] (size_t index)
  {
    return proxy(this, index);
  }

  using std::string::operator [];

  void undo()
  {
    if(old_pos == -1)
      throw std::runtime_error("Nothing to undo!");

    std::string::at(old_pos) = old;
    old = 0;
    old_pos = -1;
  }
};

int main(void)
{
  UndoString ustr("More C++ Idioms");
  std::cout << ustr[0];  // Prints 'M'
  ustr[0] = 'm';         // Change 'M' to 'm'
  std::cout << ustr[0];  // Prints 'm'
  ustr.undo();           // Restore'M'
  std::cout << ustr[0];  // Prints 'M'
}

```

The new `operator []` returns an object of `proxy` type. The `proxy` type defines an overloaded assignment operator and a conversion operator. Depending on the context how the proxy is used, the compiler chooses different functions as shown below.

> NOTE: 这段话是非常重要的，它对应是我在Motivation段的总结。

In all the output expressions (`std::cout`) above, the `proxy` object is used for reading and therefore, the compiler uses the conversion operator, which simply returns the underlying character. In the assignment statement (`ustr[0] = 'm';`), however, the compiler invokes the assignment operator of the `proxy` class. The assignment operator of the `proxy` object saves the original character value in the parent `UndoString` object and finally writes the new character value to the new position. 

> NOTE: `struct proxy`的`void operator = (const char & rhs)`用于write，`operator const char & () const`用于read。

This way, using an extra level of indirection of a temporary proxy object the idiom is able to distinguish between a read and a write operation and take different action based on that.

### Caveats

Introducing an intermediary `proxy` may result in surprising compiler errors. For example, a seemingly innocuous function `modify` fails to compile using the current definition of the `proxy` class.

```c++
#include <iostream>
#include <string>
#include <stdexcept>
class UndoString : public std::string
{
  struct proxy
  {
    UndoString * str;
    size_t pos;

    proxy(UndoString * us, size_t position)
      : str(us), pos(position)
    {}

    // Invoked when proxy is used to modify the value.
    void operator = (const char & rhs) 
    {
      str->old = str->at(pos);
      str->old_pos = pos;
      str->at(pos) = rhs;
    }

    // Invoked when proxy is used to read the value.
    operator const char & () const
    {
      return str->at(pos);
    }
  };

  char old;
  int old_pos;

public:

  UndoString(const std::string & s)
    : std::string(s), old(0), old_pos(-1)
  {}

  // This operator replaces std::string's non-const operator [].
  proxy operator [] (size_t index)
  {
    return proxy(this, index);
  }

  using std::string::operator [];

  void undo()
  {
    if(old_pos == -1)
      throw std::runtime_error("Nothing to undo!");

    std::string::at(old_pos) = old;
    old = 0;
    old_pos = -1;
  }
};
void modify(char &c)
{
  c = 'Z';
}

int main(void)
{
  UndoString ustr("More C++ Idioms");
  modify(ustr[0]);
}

```

> NOTE: 上述代码编译报错如下：
>
> ```
> test.cpp: In function ‘int main()’:
> test.cpp:65:17: error: invalid initialization of non-const reference of type ‘char&’ from an rvalue of type ‘UndoString::proxy’
>    modify(ustr[0]);
> ```
>
> 

The compiler is unable to find a conversion operator that converts the temporary proxy object into a `char &`. We have defined only a `const` conversion operator that returns a `const char &`. To allow the program to compile, another non-const conversion operator could be added. However, as soon as we do that, it is not clear whether the result of conversion is used to modify the original or not.

> NOTE: 添加一个non-const conversion operator就导致无法区分是用于read还是write，这就违背了上面的意图了。

​	