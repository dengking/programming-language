# 关于本章

本章描述在C和C++语言中都会涉及到的`static`，本章是一个专题总结。本文主要分为两大块：

- static function
- static object

虽然上面将两者分开，但是实际编程中，可能会出现两者之的重叠：static object in static function。

## `static` keyword 

https://www.geeksforgeeks.org/static-keyword-cpp/

## static function

对于static function，需要考虑的问题有：

- linkage

### What is static function

首先描述static function的概念。

https://stackoverflow.com/questions/558122/what-is-a-static-function-in-c

### static function in header



https://stackoverflow.com/questions/780730/c-c-static-function-in-header-file-what-does-it-mean



### static inline function in header

https://stackoverflow.com/questions/47819719/static-inline-functions-in-a-header-file

https://stackoverflow.com/questions/22102919/static-vs-inline-for-functions-implemented-in-header-files?noredirect=1

https://stackoverflow.com/questions/12836171/difference-between-an-inline-function-and-static-inline-function

## static object

对于static object，需要考虑的问题有：

- storage duration / lifetime
- initialization
- 唯一性






### c++ static const members

https://cookierobotics.com/032/

https://stackoverflow.com/questions/3531060/how-to-initialize-a-static-const-member-in-c

https://en.cppreference.com/w/cpp/language/static#Constant_static_members

https://stackoverflow.com/questions/29822181/prevent-static-initialization-order-fiasco-c

https://stackoverflow.com/questions/12247912/extern-vs-singleton-class


https://isocpp.org/wiki/faq/ctors#static-init-order


https://isocpp.org/wiki/faq/ctors#construct-on-first-use-v2


https://stackoverflow.com/questions/14495536/how-to-initialize-const-member-variable-in-a-class

### Initialization



https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Runtime_Static_Initialization_Order_Idioms



#### Construct on first use idiom
https://stackoverflow.com/questions/25122575/construct-on-first-use-forced-initialization-to-solve-static-initialization-or

http://www.parashift.com/c++-faq/construct-on-first-use-v2.html

### static local object in static function



### 唯一性

#### Nifty counter idiom

https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Nifty_Counter

#### Function-local static objects in inline function

在`cppreference Storage class specifiers#Static local variables`中对这个问题进行了说明:

> Function-local static objects in all definitions of the same [inline function](inline.html) (which may be implicitly inline) all refer to the same object defined in one translation unit.

TO READ:

https://stackoverflow.com/questions/185624/static-variables-in-an-inlined-function