

## summary of undefined behavior

http://www.eskimo.com/~scs/readings/undef.950311.html

### [1](https://blog.regehr.org/archives/959)

if you cast a pointer to a different kind of pointer and then dereference it, then your program has executed an [undefined behavior](http://blog.regehr.org/archives/213). 

这种情况的一个典型例子就是：[Sockets example](https://en.wikipedia.org/wiki/Type_punning)

其实上面这段话的意思和[What is the Strict Aliasing Rule and Why do we care?](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8)中所描述的是一致的：

In C and `C++` **aliasing** has to do with what **expression types** we are allowed to access stored values through. In both C and `C++` the standard specifies which **expression types** are allowed to alias which types. The compiler and optimizer are allowed to assume we follow the **aliasing rules** strictly, hence the term *strict aliasing rule*. If we attempt to access a **value** using a **type** not allowed it is classified as [undefined behavior](http://en.cppreference.com/w/cpp/language/ub)(**UB**). Once we have **undefined behavior** all bets are off, the results of our program are no longer reliable.

### 6.2.4 Storage durations of objects

The lifetime of an object is the portion of program execution during which storage is guaranteed to be reserved for it. An object exists, has a constant address, 33) and retains its last-stored value throughout its lifetime. 34) If an object is referred to outside of its lifetime, the behavior is undefined. The value of a pointer becomes indeterminate when the object it points to (or just past) reaches the end of its lifetime.



### J.2 Undefined behavior

