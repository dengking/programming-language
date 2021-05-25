# howardhinnant [Incomplete types and `shared_ptr` / `unique_ptr`](http://howardhinnant.github.io/incomplete.html)

Most templates in the C++ standard library require that they be instantiated with complete types. However `shared_ptr` and `unique_ptr` are *partial* exceptions. Some, but not all of their members can be instantiated with incomplete types. The motivation for this is to support idioms such as [*pimpl*](http://en.wikipedia.org/wiki/Opaque_pointer#C.2B.2B) using smart pointers, and without risking undefined behavior.

> NOTE: 
> 1、其实是为了能够覆盖opaque pointer

Undefined behavior can occur when you have an incomplete type and you call `delete` on it:

> ```c++
> class A;
> A* a = ...;
> delete a;
> ```

The above is legal code. It will compile. Your compiler may or may not emit a warning for above code like the above. When it executes, bad things will probably happen. If you're very lucky your program will crash. However a more probable outcome is that your program will silently leak memory as `~A()` won't be called.

Using `auto_ptr<A>` in the above example doesn't help. You still get the same undefined behavior as if you had used a raw pointer.

Nevertheless, using incomplete classes in certain places is *very* useful! This is where `shared_ptr` and `unique_ptr` help. Use of one of these smart pointers will let you get away with an incomplete type, except where it is neccessary to have a complete type. And most importantly, when it *is* necessary to have a complete type, you get a compile-time error if you try to use the smart pointer with an incomplete type at that point.

However, in case it is helpful to you, here is a table which documents several members of `shared_ptr` and `unique_ptr` with respect to completeness requirements. If the member requires a complete type, then entry has a "C", otherwise the table entry is filled with "I". I've also added a column for `noexcept` (True or False) for each operation.

> |                                       | `unique_ptr` |            | `shared_ptr` |            |
> | ------------------------------------- | ------------ | ---------- | ------------ | ---------- |
> |                                       | I/C          | `noexcept` | I/C          | `noexcept` |
> | `P()` default constructor             | I            | T          | I            | T          |
> | `P(const P&)` copy constructor        | N/A          | N/A        | I            | T          |
> | `P(P&&)` move constructor             | I            | T          | I            | T          |
> | `~P()` destructor                     | C            | T          | I            | T          |
> | `P(A*)`                               | I            | T          | C            | F          |
> | `operator=(const P&)` copy assignment | N/A          | N/A        | I            | T          |
> | `operator=(P&&)` move assignment      | C            | T          | I            | T          |
> | `reset()`                             | C            | T          | I            | T          |
> | `reset(A*)`                           | C            | T          | C            | F          |

Any operations requiring pointer conversions require complete types for both `unique_ptr` and `shared_ptr`.