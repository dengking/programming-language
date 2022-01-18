# Unified assignment operator

这是在 stackoverflow [What are move semantics?](https://stackoverflow.com/questions/3106110/what-are-move-semantics) # [part two](https://stackoverflow.com/a/11540204) # Special member functions 中提出的:

```C++
X& X::operator=(X source)    // unified assignment operator
{
    swap(source);            // see my first answer for an explanation
    return *this;
}
```

它有利也有弊:

弊端:

1、子类中，如果要调用基类的`operator=`，要如何实现呢？

