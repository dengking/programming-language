[TOC]







# [What is a “static” function?](https://stackoverflow.com/questions/558122/what-is-a-static-function)

The question was about plain [c](https://stackoverflow.com/questions/tagged/c) functions, not [c++](https://stackoverflow.com/questions/tagged/c%2b%2b) `static` methods, as clarified in comments.

Ok, I understand what a `static` variable is, but what is a `static` function?

And why is it that if I declare a function, let's say `void print_matrix`, in let's say `a.c` (WITHOUT `a.h`) and include `"a.c"` - I get `"print_matrix@@....) already defined in a.obj"`, BUT if I declare it as `static void print_matrix` then it compiles?

**UPDATE** Just to clear things up - I know that including `.c` is bad, as many of you pointed out. I just do it to temporarily clear space in `main.c` until I have a better idea of how to group all those functions into proper `.h` and `.c` files. Just a temporary, quick solution.



## [A](https://stackoverflow.com/a/558132)

`static` functions are functions that are only visible to other functions in the same file (more precisely *the same translation unit*).

**EDIT**: For those who thought, that the author of the questions meant a 'class method': As the question is tagged `C` he means a plain old C function. For (C++/Java/...) class methods, `static`means that this method can be called on the class itself, no instance of that class necessary.



## [A](https://stackoverflow.com/a/558201)

There is a big difference between static functions in C and static member functions in C++. In C, a static function is not visible outside of its translation unit, which is the object file it is compiled into. In other words, making a function static limits its scope. You can think of a static function as being "private" to its *.c file (although that is not strictly correct).

In C++, "static" can also apply to member functions and data members of classes. A static data member is also called a "class variable", while a non-static data member is an "instance variable". This is Smalltalk terminology. This means that there is only one copy of a static data member shared by all objects of a class, while each object has its own copy of a non-static data member. So a static data member is essentially a global variable, that is a member of a class.

Non-static member functions can access all data members of the class: static and non-static. Static member functions can only operate on the static data members.

One way to think about this is that in C++ static data members and static member functions do not belong to any object, but to the entire class.