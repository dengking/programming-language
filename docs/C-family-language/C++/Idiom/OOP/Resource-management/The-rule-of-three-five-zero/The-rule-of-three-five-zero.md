# The rule of three/five/zero



## drdobbs [C++ Made Easier: The Rule of Three](https://www.drdobbs.com/c-made-easier-the-rule-of-three/184401400)

### An Example and a Misconception

[example code](./Code/drdobbs/IntVec-Misconception.cpp)

> NOTE: 原文给出的这个例子是非常具有代表性的。
>
> 这个例子，导致错误是double free

## cppreference [The rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three)

### Rule of three

If a class requires a user-defined [destructor](https://en.cppreference.com/w/cpp/language/destructor), a user-defined [copy constructor](https://en.cppreference.com/w/cpp/language/copy_constructor), or a user-defined [copy assignment operator](https://en.cppreference.com/w/cpp/language/as_operator), it almost certainly requires all three.

> NOTE: 三者只要有其一，就需要有其三



[example code](./Code/cppreference/rule_of_three.cpp)



Classes that manage non-copyable resources through copyable handles may have to declare **copy assignment** and **copy constructor** private and not provide their definitions or define them as deleted. This is another application of the rule of three: deleting one and leaving the other to be implicitly-defined will most likely result in errors.

> NOTE: 原文的这段话要如何理解？