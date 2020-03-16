# The rule of three/five/zero



## cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three)

### Rule of three

If a class requires a user-defined [destructor](https://en.cppreference.com/w/cpp/language/destructor), a user-defined [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor), or a user-defined [copy assignment operator](https://en.cppreference.com/w/cpp/language/as_operator), it almost certainly requires all three.

> NOTE: 三者只要有其一，就需要有其三



[example code](./rule_of_three.cpp)