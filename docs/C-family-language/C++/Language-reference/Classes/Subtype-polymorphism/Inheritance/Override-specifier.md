# override specifier



## cppreference [override specifier (since C++11)](https://en.cppreference.com/w/cpp/language/override)



## Purpose of `override`

参考：

- [What is the 'override' keyword in C++ used for? [duplicate]](https://stackoverflow.com/questions/18198314/what-is-the-override-keyword-in-c-used-for)
- [Is the 'override' keyword just a check for a overridden virtual method?](https://stackoverflow.com/questions/13880205/is-the-override-keyword-just-a-check-for-a-overridden-virtual-method)



### [A](https://stackoverflow.com/a/18198377)

The `override` keyword serves two purposes:

1. It shows the reader of the code that "this is a virtual method, that is overriding a virtual method of the base class."
2. The compiler also knows that it's an override, so it can "check" that you are not altering/adding new methods that you think are overrides.

To explain the latter:

```cpp
class base
{
  public:
    virtual int foo(float x) = 0; 
};


class derived: public base
{
   public:
     int foo(float x) override { ... } // OK
}

class derived2: public base
{
   public:
     int foo(int x) override { ... } // ERROR
};
```

In `derived2` the compiler will issue an error for "changing the type". Without `override`, at most the compiler would give a warning for "you are hiding virtual method by same name".