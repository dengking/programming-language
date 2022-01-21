# linkage and linker

关于linkage和linker，在geeksforgeeks [Internal Linkage and External Linkage in C](https://www.geeksforgeeks.org/internal-linkage-external-linkage-c/)中给出了非常详细的分析；



### stackoverflow [How does linker handle variables with different linkages?](https://stackoverflow.com/questions/51737002/how-does-linker-handle-variables-with-different-linkages)

In C and C++ we can manipulate a variable's **linkage**. There are three kinds of linkage: **no linkage**, **internal linkage**, and **external linkage**. My question is probably related to why these are called "linkage" (How is that related to the **linker**).

I understand a **linker** is able to handle variables with **external linkage**, because references to this variable is not confined（限制于） within a single **translation unit**, therefore not confined within a single **object file**. How that actually works under the hood is typically discussed in courses on operating systems.

But how does the linker handle variables (1) with no linkage and (2) with internal linkage? What are the differences in these two cases?

***COMMENTS*** 

The **linker** does not handle variables with internal or no linkage. The compiler can generate code to access those types of variables without any help from the linker. (Linkage and linker are not closely related concepts.) – [markgz](https://stackoverflow.com/users/583570/markgz) [Aug 8 '18 at 0:42](https://stackoverflow.com/questions/51737002/how-does-linker-handle-variables-with-different-linkages#comment90434932_51737002)



#### [A](https://stackoverflow.com/a/51737215)

The **linker** isn't normally involved in either **internal linkage** or **no linkage**--they're resolved entirely by the **compiler**, before the **linker** gets into the act at all.

**Internal linkage** means two declarations at different scopes in the same **translation unit** can refer to the same thing.

##### No Linkage

No linkage means two declarations at different scopes in the same translation unit can't refer to the same thing.

So, if I have something like:

```c
int f() { T
    static int x; // no linkage
}
```

...no other declaration of `x` in any other scope can refer to this `x`. The linker is involved only to the degree that it typically has to produce a field in the executable telling it the **size** of static space needed by the executable, and that will include space for this variable. Since it can never be referred to by any other declaration, there's no need for the linker to get involved beyond that though (in particular, the linker has nothing to do with resolving the name).

##### Internal linkage

Internal linkage means declarations at different scopes in the same translation unit can refer to the same object. For example:

```C++
static int x;  // a namespace scope, so `x` has internal linkage

int f() { 
    extern int x; // declaration in one scope
}

int g() { 
    extern int x; // declaration in another scope
}
```

Assuming we put these all in one file (i.e., they end up as a single translation unit), the declarations in both `f()` and `g()` refer to the same thing--the `x` that's defined as `static` at namespace scope.

For example, consider code like this:

```c++
#include <iostream>

static int x; // a namespace scope, so `x` has internal linkage

int f()
{
    extern int x;
    ++x;
}

int g()
{
    extern int x;
    std::cout << x << '\n';
}

int main() {
    g();
    f();
    g();
}
```

This will print:

```
0
1
```

...because the `x` being incremented in `f()` is the same `x` that's being printed in `g()`.

The linker's involvement（参与） here can be (and usually is) pretty much the same as in the **no linkage** case--the variable `x` needs some space, and the linker specifies that space when it creates the executable. It does *not*, however, need to get involved in determining that when `f()` and `g()` both declare `x`, they're referring to the same `x`--the compiler can determine that.

We can see this in the generated code. For example, if we compile the code above with `gcc`, the relevant bits for `f()` and `g()` are these.

f:

```assembly
    movl    _ZL1x(%rip), %eax
    addl    $1, %eax
    movl    %eax, _ZL1x(%rip)
```

That's the increment of `x` (it uses the name `_ZL1x` for it).

g:

```assembly
    movl    _ZL1x(%rip), %eax
    [...]
    call    _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_c@PLT
```

So that's basically loading up `x`, then sending it to `std::cout` (I've left out code for other parameters we don't care about here).

The important part is that the code refers to `_ZL1x`--the same name as `f` used, so both of them refer to the same object.

The **linker** isn't really involved, because all it sees is that this file has requested space for one statically allocated variable. It makes space for that, but doesn't have to do anything to make `f` and `g` refer to the same thing--that's already handled by the compiler.



#### [A](https://stackoverflow.com/a/51737185)

As far as C++ itself goes, this does not matter: the only thing that matters is the behavior of the system as a whole. Variables with no linkage should not be linked; variables with internal linkage should not be linked across translation units; and variables with external linkage should be linked across translation units. (Of course, as the person writing the C++ code, you must obey all of *your* constraints as well.)

Inside a compiler and linker suite of programs, however, we certainly *do* have to care about this. The method by which we achieve the desired result is up to us. One traditional method is pretty simple:

- Identifiers with no linkage are never even passed through to the linker.
- Identifiers with internal linkage are not passed through to the linker either, or *are* passed through to the linker but marked "for use within this one translation unit only". That is, there is no `.global`declaration for them, or there is a `.local` declaration for them, or similar.
- Identifiers with external linkage are passed through to the linker, and if internal linkage identifiers are seen by the linker, these external linkage symbols are marked differently, e.g., have a `.global` declaration or no `.local` declaration.

If you have a Linux or Unix like system, run `nm` on object (`.o`) files produced by the compiler. Note that some symbols are annotated with uppercase letters like `T` and `D` for text and data: these are global. Other symbols are annotated with lowercase letters like `t` and `d`: these are local. So these systems are using the "pass internal linkage to the linker, but mark them differently from external linkage" method.

