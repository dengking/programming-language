# Construct On First Use

## [More C++ Idioms/Construct On First Use](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Construct_On_First_Use)

```c++
#include <iostream>
using namespace std;
struct Bar {
Bar () {
cout << "Bar::Bar()\n";
}
void f () {
cout << "Bar::f()\n";
}
};
struct Foo {
Foo () {
bar_.f ();
}
static Bar bar_;
};
Foo f;
Bar Foo::bar_;
int main () {}
```



## isocpp What’s the “`static` initialization order ‘fiasco’ (problem)”? [¶](https://isocpp.org/wiki/faq/ctors#static-init-order) [Δ](https://isocpp.org/wiki/faq/ctors#)