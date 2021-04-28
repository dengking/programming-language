# Assignment



## cppreference [Copy assignment operator](https://en.cppreference.com/w/cpp/language/copy_assignment)



## cppreference [Move assignment operator](https://en.cppreference.com/w/cpp/language/move_assignment)







## Calling operators of base class

### stackoverflow [calling operators of base class… safe?](https://stackoverflow.com/questions/4734846/calling-operators-of-base-class-safe)



[A](https://stackoverflow.com/a/4734889)

This is fine, but it's a lot more readable IMHO to call the base-class by name:

```cpp
Base::operator = (rhs);
```

