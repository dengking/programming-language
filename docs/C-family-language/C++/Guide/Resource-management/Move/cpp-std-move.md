

## [What is std::move(), and when should it be used?](https://stackoverflow.com/questions/3413470/what-is-stdmove-and-when-should-it-be-used)

## [A](https://stackoverflow.com/a/3413547)

[Wikipedia Page on C++11 R-value references and move constructors](https://en.wikipedia.org/wiki/C%2B%2B11#Rvalue_references_and_move_constructors)

1. In C++11, in addition to copy constructors, objects can have move constructors.
   (And in addition to copy assignment operators, they have move assignment operators.)
2. The move constructor is used instead of the copy constructor, if the object has type "rvalue-reference" (`Type &&`).
3. `std::move()` is a cast that produces an **rvalue-reference** to an object, to enable moving from it.

It's a new `C++` way to **avoid copies**. For example, using a move constructor, a `std::vector` could just copy its internal pointer to data to the new object, leaving the moved object in an incorrect state, avoiding to copy all data. This would be C++-valid.

Try googling for move semantics, rvalue, perfect forwarding.



## cppreference [std::move](https://en.cppreference.com/w/cpp/utility/move)

