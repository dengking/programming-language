# Counted Body



## More C++ Idioms/[Counted Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Counted_Body)

> NOTE: 这个idiom是对[Handle/Body](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Handle_Body#Pointer_To_Implementation_(pImpl)) idiom的完善，通过reference counting技术来实现automatic resource management，所以它名为counted body。

### Solution and Sample Code

The solution is to add a **reference count** to the **body class** to facilitate memory management; hence the name "Counted Body." Memory management is added to the **handle class**, particularly to its implementation of initialization, assignment, copying, and destruction.

```c++
#include <algorithm>
#include <iostream>
#include <cstring>
class StringRep {
  friend class String;

  friend std::ostream &operator<<(std::ostream &out, StringRep const &str) {
    out << "[" << str.data_ << ", " << str.count_ << "]";
    return out;
  }

 public:
  StringRep(const char *s) : count_(1) {
    strcpy(data_ = new char[strlen(s) + 1], s);
  }

  ~StringRep() { delete[] data_; }

 private:
  int count_;
  char *data_;
};

class String {
 public:
  String() : rep(new StringRep("")) {
    std::cout << "empty ctor: " << *rep << "\n";
  }
  String(const String &s) : rep(s.rep) {
    rep->count_++;
    std::cout << "String ctor: " << *rep << "\n";
  }
  String(const char *s) : rep(new StringRep(s)) {
    std::cout << "char ctor:" << *rep << "\n";
  }
  String &operator=(const String &s) {
    std::cout << "before assign: " << *s.rep << " to " << *rep << "\n";
    String(s).swap(*this);  // copy-and-swap idiom
    std::cout << "after assign: " << *s.rep << " , " << *rep << "\n";
    return *this;
  }
  ~String() {  // StringRep deleted only when the last handle goes out of scope.
    if (rep && --rep->count_ <= 0) {
      std::cout << "dtor: " << *rep << "\n";
      delete rep;
    }
  }

 private:
  void swap(String &s) throw() { std::swap(this->rep, s.rep); }

  StringRep *rep;
};
int main() {

  std::cout << "*** init String a with empty\n";
  String a;
  std::cout << "\n*** assign a to \"A\"\n";
  a = "A";

  std::cout << "\n*** init String b with \"B\"\n";
  String b = "B";

  std::cout << "\n*** b->a\n";
  a = b;

  std::cout << "\n*** init c with a\n";
  String c(a);

  std::cout << "\n*** init d with \"D\"\n";
  String d("D");

  return 0;
}
```