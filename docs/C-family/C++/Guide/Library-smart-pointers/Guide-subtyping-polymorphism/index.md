# Smart pointer and subtyping polymorphism

1、C++ subtyping polymorphism的实现是依赖于reference semantic的

2、pointer用于subtyping polymorphism，显然smart pointer也需要能够支持subtyping polymorphism

3、关于此的例子:

a、`nextptr-shared_ptr-basics-and-internals` 章节