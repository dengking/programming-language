# By value and by reference

## Pass-by-value and pass-by-reference

> NOTE: tag-Value semantic and reference semantic-pass

这两者是最最能够体现value-semantic和reference-semantic的。

## Call-by-value and call-by-reference-pointer

> NOTE: tag-Value semantic and reference semantic-call

1、C++中，只有call-by-reference-pointer才能够触发subtyping/dynamic polymorphism。

在 `Subtyping-polymorphism # Reference semantic and runtime polymorphism` 段中对此进行了非常详细的说明。

2、

|      | Call-by-value | Call-by-reference-pointer |
| ---- | ------------- | ------------------------- |
|      | static bind   | dynamic bind              |
|      | static type   | dynamic type              |
|      |               |                           |

下面是说明: 

Call-by-value 对应的是 static bind ，使用的是 static type

Call-by-reference-pointer 对应的是 dynamic bind，使用的是 dynamic type

谈及这个topic的文章:

1、 stackoverflow [What is object slicing?](https://stackoverflow.com/questions/274626/what-is-object-slicing) # [A](https://stackoverflow.com/a/14461532) 中进行了详细介绍

2、nextptr [shared_ptr - basics and internals with examples](https://www.nextptr.com/tutorial/ta1358374985/shared_ptr-basics-and-internals-with-examples)

其中谈及了"Call-by-value and call-by-reference-pointer"，已经收录了

## TODO Return-by-value and return-by-reference

> NOTE: NOTE: tag-Value semantic and reference semantic-return

## draft

### By value and by reference

显然by value涉及到copy，并且构造的对象在函数调用结束后就会被release掉；但是by reference则不存在。

> This is not just a matter of convenience but in fact an optimization. If the parameter (s) binds to an **lvalue** (another non-const object), a copy of the object is made automatically while creating the parameter (s). However, when s binds to an rvalue (temporary object, literal), the copy is typically elided, which saves a call to a copy constructor and a destructor. In the earlier version of the assignment operator where the parameter is accepted as **const reference**, **copy elision** does not happen when the **reference** binds to an **rvalue**. This results in an additional object being created and destroyed.

