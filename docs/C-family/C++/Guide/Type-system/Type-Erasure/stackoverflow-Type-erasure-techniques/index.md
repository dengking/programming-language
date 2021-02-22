# stackoverflow [Type erasure techniques](https://stackoverflow.com/questions/5450159/type-erasure-techniques)

(With type erasure, I mean hiding some or all of the type information regarding a class, somewhat like [Boost.Any](http://www.boost.org/doc/libs/1_46_0/doc/html/any.html).)

The first and most obvious, and commonly taken approach, that I know, are **virtual functions**. Just hide the implementation of your class inside an interface based class hierarchy. Many Boost libraries do this, for example [Boost.Any](http://www.boost.org/doc/libs/1_46_0/doc/html/any.html) does this to hide your type and [Boost.Shared_ptr](http://www.boost.org/doc/libs/1_46_1/libs/smart_ptr/shared_ptr.htm) does this to hide the (de)allocation mechanic.

> NOTE: 参见 "Virtual function、consistent type、consistent interface" 段

Then there is the option with function pointers to templated functions, while holding the actual object in a `void*` pointer, like [Boost.Function](http://www.boost.org/doc/libs/1_46_1/doc/html/function.html) does to hide the real type of the functor. Example implementations can be found at the end of the question.

> NOTE: function pointer