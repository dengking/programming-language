# Temporary Proxy



## More C++ Idioms/[Temporary Proxy](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Temporary_Proxy)



### Motivation

The index operator (`operator []`) is often used to provide array like access syntax for user-defined classes. C++ standard library uses `operator []` in std::string and std::map classes. Standard string simply returns a character reference as a result of `operator []` whereas std::map returns a reference to the value given its key. In both cases, the returned reference can be directly read or written to. The string or map class have no knowledge or has no control over whether the reference is used for reading or for modification. Sometimes, however, it is useful to detect how the value is being used.

For example, consider an `UndoString` class, which supports a single *undo* operation in addition to the existing `std::string` interface. The design must allow an undo even though the character is accessed using the index operator. As mention above, `std::string` class has no knowledge of whether the result of `operator []` will be written to or not. The temporary proxy can be used to solve this problem.

### Solution and Sample Code

The temporary proxy idiom uses another object, conveniently called *proxy*, to detect whether the result of `operator []` is used for reading or writing. The `UndoString` class below defines its own non-const `operator []`, which takes place of `std::string`'s non-const `operator []`.



```c++
#include <iostream>
#include <string>
#include <exception>
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

