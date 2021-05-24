# Copyable、movable and the rule of three/five/zero



## Rule of three/five/zero

## Resource wrapper: Single responsibility +  rule of three five zero + RAII 

使用"Single responsibility +  rule of three five zero + RAII"来实现Resource wrapper。

## cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three)

### Rule of three

If a class requires a user-defined [destructor](https://en.cppreference.com/w/cpp/language/destructor), a user-defined [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor), or a user-defined [copy assignment operator](https://en.cppreference.com/w/cpp/language/as_operator), it almost certainly requires all three.

### Rule of five

Because the presence of a user-defined destructor, copy-constructor, or copy-assignment operator prevents implicit definition of the [move constructor](https://en.cppreference.com/w/cpp/language/move_constructor) and the [move assignment operator](https://en.cppreference.com/w/cpp/language/move_operator), any class for which move semantics are desirable, has to declare all five special member functions

### How to implementation

## stackoverflow [Rule of Four (and a half)?](https://stackoverflow.com/questions/45754226/what-is-the-rule-of-four-and-a-half)

