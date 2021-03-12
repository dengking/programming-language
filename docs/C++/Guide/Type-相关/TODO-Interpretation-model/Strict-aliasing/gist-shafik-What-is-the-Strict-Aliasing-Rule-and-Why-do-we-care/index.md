# [What is the Strict Aliasing Rule and Why do we care?](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8)

## (OR Type Punning, Undefined Behavior and Alignment, Oh My!)

What is strict aliasing? First we will describe what is aliasing and then we can learn what being **strict** about it means.

> NOTE: 在阅读本文之前，需要首先搞清楚[Aliasing (computing)](https://en.wikipedia.org/wiki/Aliasing_(computing))的含义；

In C and `C++` **aliasing** has to do with what **expression types** we are allowed to access stored values through. In both C and `C++` the standard specifies which **expression types** are allowed to alias which types. The compiler and optimizer are allowed to assume we follow the **aliasing rules** strictly, hence the term *strict aliasing rule*. If we attempt to access a **value** using a **type** not allowed it is classified as [undefined behavior](http://en.cppreference.com/w/cpp/language/ub)(**UB**). Once we have undefined behavior all bets are off, the results of our program are no longer reliable.

> ***TRANSLATION*** : 在C和C ++中，**别名**与我们允许通过哪些表达式类型访问存储的值有关。C和`C++`的标准都指定了哪种**expression types** 允许去alias哪种type；

Unfortunately with strict aliasing violations, we will often obtain the results we expect, leaving the possibility the a future version of a compiler with a new optimization will break code we thought was valid. This is undesirable and it is a worthwhile goal to understand the strict aliasing rules and how to avoid violating them.

> ***TRANSLATION*** : 不幸的是，由于严格的别名违规，我们经常会得到我们期望的结果，而新的优化版本的编译器的未来版本可能会破坏我们认为有效的代码。 这是不可取的，理解严格的别名规则以及如何避免违反它们是一个值得的目标。

To understand more about why we care, we will discuss issues that come up when violating strict aliasing rules, [type punning](https://en.wikipedia.org/wiki/Type_punning) since common techniques used in type punning often violate strict aliasing rules and how to type pun correctly, along with some possible help from `C++20` to make type punning simpler and less error prone. We will wrap up the discussion by going over some methods for catching strict aliasing violations.

### Preliminary examples

Let's look at some examples, then we can talk about exactly what the standard(s) say, examine some further examples and then see how to avoid strict aliasing and catch violations we missed. Here is an example that should not be surprising ([live example](https://wandbox.org/permlink/7sCJTAyrifZ0zfFA)):

```c
int x = 10;
int *ip = &x;
    
std::cout << *ip << "\n";
*ip = 12;
std::cout << x << "\n";
```

We have a `int*` pointing to memory occupied by an *int* and this is a valid aliasing. The optimizer must assume that assignments through **ip** could update the value occupied by **x**.

The next example shows **aliasing** that leads to **undefined behavior** ([live example](https://wandbox.org/permlink/8qA8JyJRVHtS9LPf)):

```c++
int foo( float *f, int *i ) { 
    *i = 1;               
    *f = 0.f;            
   
   return *i;
}

int main() {
    int x = 0;
    
    std::cout << x << "\n";   // Expect 0
    x = foo(reinterpret_cast<float*>(&x), &x);
    std::cout << x << "\n";   // Expect 0?
}
```

In the function **foo** we take an `int*` and a `float*`, in this example we call **foo** and set both parameters to point to the same memory location which in this example contains an *int*. Note, the [reinterpret_cast](http://en.cppreference.com/w/cpp/language/reinterpret_cast) is telling the compiler to treat the the expression as if it had the type specificed by its template parameter. In this case we are telling it to treat the expression **&x** as if it had type `*float`**. We may naively expect the result of the second **`cout` to be **0** but with optimization enabled using **-O2** both `gcc` and `clang` produce the following result:

```
0
1
```

Which may not be expected but is perfectly valid since we have invoked **undefined behavior**. A *float* can not validly **alias** an *int* object. Therefore the optimizer can assume the *constant 1* stored when dereferencing **i** ( dereferencing **i**  的含义是`*i`) will be the return value since a store through **f** could not validly affect an *int* object. Plugging the code in Compiler Explorer shows this is exactly what is happening([live example](https://godbolt.org/g/yNV5aj)):

```assembly
foo(float*, int*): # @foo(float*, int*)
mov dword ptr [rsi], 1  
mov dword ptr [rdi], 0
mov eax, 1                       
ret
```

The optimizer using Type-Based Alias Analysis (TBAA)[6](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f6) assumes **1** will be returned and directly moves the constant value into register **eax** which carries the return value. TBAA uses the languages rules about **what types are allowed to alias** to optimize **loads and stores**. In this case TBAA knows that a *float* can not alias and *int* and optimizes away the load of **i**.

> NOTE: 需要Google store and load

## Now, to the Rule-Book

What exactly does the standard say we are allowed and not allowed to do? The standard language is not straightforward, so for each item I will try to provide code examples that demonstrates the meaning.

### What does the C11 standard say?

The **C11** standard[2](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f2) says the following in section *6.5 Expressions paragraph 7*:

> An object shall have its stored value accessed only by an **lvalue expression** [5](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f5) that has one of the following types:88) — a type compatible with the **effective type** of the **object**,

```c
int x = 1;
int *p = &x;   
printf("%d\n", *p); // *p gives us an lvalue expression of type int which is compatible with int
```

> NOTE : `*p`是一个`lvalue expression`，它的类型是`int`。

> — a qualified version of a type compatible with the effective type of the object,

```c
int x = 1;
const int *p = &x;
printf("%d\n", *p); // *p gives us an lvalue expression of type const int which is compatible with int
```

> — a type that is the signed or unsigned type corresponding to the effective type of the object,

```c
int x = 1;
unsigned int *p = (unsigned int*)&x;
printf("%u\n", *p ); // *p gives us an lvalue expression of type unsigned int which corresponds to 
                     // the effective type of the object
```

[See Footnote 12 for gcc/clang extension](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f12), that allows assigning `unsigned int*` to `int*` even though they are not compatible types.

> — a type that is the signed or unsigned type corresponding to a qualified version of the effective type of the object,

```c
int x = 1;
const unsigned int *p = (const unsigned int*)&x;
printf("%u\n", *p ); // *p gives us an lvalue expression of type const unsigned int which is a unsigned type 
                     // that corresponds with to a qualified verison of the effective type of the object
```

> — an aggregate or union type that includes one of the aforementioned types among its members (including, recursively, a member of a subaggregate or contained union), or

```c
struct foo {
  int x;
};
    
void foobar( struct foo *fp, int *ip );  // struct foo is an aggregate that includes int among its members so it can
                                         // can alias with *ip

foo f;
foobar( &f, &f.x );
```

> — a character type.

```c
int x = 65;
char *p = (char *)&x;
printf("%c\n", *p );  // *p gives us an lvalue expression of type char which is a character type.
                      // The results are not portable due to endianness issues.
```



> NOTE : 这部分内容在[cpppreference](https://en.cppreference.com/w/c/language/object)的[Strict aliasing](https://en.cppreference.com/w/c/language/object)中有介绍；

### What the C++17 Draft Standard say

The C++17 draft standard[3](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f3) in section *[basic.lval] paragraph 11* says:

> If a program attempts to access the stored value of an object through a glvalue of other than one of the following types the behavior is undefined:63 (11.1) — the dynamic type of the object,

```
void *p = malloc( sizeof(int) ); // We have allocated storage but not started the lifetime of an object
int *ip = new (p) int{0};        // Placement new changes the dynamic type of the object to int
std::cout << *ip << "\n";        // *ip gives us a glvalue expression of type int which matches the dynamic type 
                                  // of the allocated object
```

> (11.2) — a cv-qualified version of the dynamic type of the object,

```
int x = 1;
const int *cip = &x;
std::cout << *cip << "\n";  // *cip gives us a glvalue expression of type const int which is a cv-qualified 
                            // version of the dynamic type of x
```

> (11.3) — a type similar (as defined in 7.5) to the dynamic type of the object,

```
// Need an example for this
```

> (11.4) — a type that is the signed or unsigned type corresponding to the dynamic type of the object,

```
// Both si and ui are signed or unsigned types corresponding to each others dynamic types
// We can see from this godbolt(https://godbolt.org/g/KowGXB) the optimizer assumes aliasing.
signed int foo( signed int &si, unsigned int &ui ) {
  si = 1;
  ui = 2;

  return si;
}
```

> (11.5) — a type that is the signed or unsigned type corresponding to a cv-qualified version of the dynamic type of the object,

```
signed int foo( const signed int &si1, int &si2); // Hard to show this one assumes aliasing
```

> (11.6) — an aggregate or union type that includes one of the aforementioned types among its elements or nonstatic data members (including, recursively, an element or non-static data member of a subaggregate or contained union),

```
struct foo {
 int x;
};

// Compiler Explorer example(https://godbolt.org/g/z2wJTC) shows aliasing assumption
int foobar( foo &fp, int &ip ) {
 fp.x = 1;
 ip = 2;

 return fp.x;
}

foo f; 
foobar( f, f.x ); 
```

> (11.7) — a type that is a (possibly cv-qualified) base class type of the dynamic type of the object,

```
struct foo { int x ; };

struct bar : public foo {};

int foobar( foo &f, bar &b ) {
  f.x = 1;
  b.x = 2;

  return f.x;
}
```

> (11.8) — a char, unsigned char, or std::byte type.

```
int foo( std::byte &b, uint32_t &ui ) {
  b = static_cast<std::byte>('a');
  ui = 0xFFFFFFFF;                   
  
  return std::to_integer<int>( b );  // b gives us a glvalue expression of type std::byte which can alias
                                     // an object of type uint32_t
}
```

Worth noting *signed char* is not included in the list above, this is a notable difference from *C* which says *a character type*.

## Subtle Differences

So although we can see that C and C++ say similar things about aliasing there are some differences that we should be aware of. C++ does not have C's concept of [effective type](http://en.cppreference.com/w/c/language/object#Effective_type) or [compatible type](http://en.cppreference.com/w/c/language/type#Compatible_types) and C does not have C++'s concept of [dynamic type](http://en.cppreference.com/w/cpp/language/type#Dynamic_type)or *similar type*. Although both have *lvalue* and *rvalue* expressions[5](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f5), C++ also has *glvalue*, *prvalue* and *xvalue*[9](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f9) expressions. These differences are mostly out of scope for this article but one interesting example is how to create an object out of malloc'd memory. In C we can set the *effective type*[10](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f10) for example by writing to the memory through an *lvalue* or **memcpy**[11](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f11).

```
// The following is valid C but not valid C++
void *p = malloc(sizeof(float));
float f = 1.0f;
memcpy( p, &f, sizeof(float));  // Effective type of *p is float in C
                                 // Or
float *fp = p;                   
*fp = 1.0f;                      // Effective type of *p is float in C
```

Neither of these methods is sufficient in C++ which requires **placement new**:

```
float *fp = new (p) float{1.0f} ;   // Dynamic type of *p is now float
```

## Are int8_t and uint8_t char types?

Theoretically neither *int8_t* nor *uint8_t* have to be *char* types but practically they are implemented that way. This is important because if they are really *char* types then they also alias similar to *char* types. If you are unaware of this it can [lead to surprising performance impacts](https://stackoverflow.com/q/26295216/1708801). We can see that glibc typedefs [int8_t](https://github.com/lattera/glibc/blob/master/sysdeps/generic/stdint.h#L36) and [uint8_t](https://github.com/lattera/glibc/blob/master/sysdeps/generic/stdint.h#L48) to *signed char* and *unsigned char*respectively.

This would be hard to change since for *C++* it would be an ABI break. This would change name mangling and would break any API using either of those types in their interface.

## What is Type Punning

We have gotten to this point and we may be wondering, why would we want to alias for? The answer typically is to *type pun*, often the methods used violate strict aliasing rules.

Sometimes we want to circumvent the type system and interpret an object as a different type. This is called *type punning*, to reinterpret a segment of memory as another type. *Type punning* is useful for tasks that want access to the underlying representation of an object to view, transport or manipulate. Typical areas we find type punning being used are compilers, serialization, networking code, etc…

Traditionally this has been accomplished by taking the address of the object, casting it to a pointer of the type we want to reinterpret it as and then accessing the value, or in other words by aliasing. For example:

```
int x =  1 ;

// In C
float *fp = (float*)&x ;  // Not a valid aliasing

// In C++
float *fp = reinterpret_cast<float*>(&x) ;  // Not a valid aliasing

printf( “%f\n”, *fp ) ;
```

As we have seen earlier this is not a valid aliasing, so we are invoking undefined behavior. But traditionally compilers did not take advantage of strict aliasing rules and this type of code usually just worked, developers have unfortunately gotten used to doing things this way. A common alternate method for type punning is through unions, which is valid in C but *undefined behavior* in C++[13](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f13) ([see live example](https://wandbox.org/permlink/oOf9bPlcWDYrYqPF)):

```
union u1
{
  int n;
  float f;
} ;

union u1 u;
u.f = 1.0f;

printf( "%d\n”, u.n );  // UB in C++ n is not the active member
```

This is not valid in C++ and some consider the purpose of unions to be solely for implementing variant types and feel using unions for type punning is an abuse.

### How do we Type Pun correctly?

The standard blessed method for *type punning* in both C and C++ is **memcpy**. This may seem a little heavy handed but the optimizer should recognize the use of **memcpy** for *type punning* and optimize it away and generate a register to register move. For example if we know *int64_t* is the same size as *double*:

```
static_assert( sizeof( double ) == sizeof( int64_t ) );  // C++17 does not require a message
```

we can use **memcpy**:

```
void func1( double d ) {
  std::int64_t n;
  std::memcpy(&n, &d, sizeof d); 
  //...
```

At a sufficient optimization level any decent modern compiler generates identical code to the previously mentioned **reinterpret_cast** method or *union* method for *type punning*. Examining the generated code we see it uses just register mov ([live Compiler Explorer Example](https://godbolt.org/g/BfZGwX)).

### Type Punning Arrays

But, what if we want to type pun an array of *unsigned char* into a series of *unsigned ints* and then perform an operation on each *unsigned int* value? We can use **memcpy** to pun the *unsigned char array* into a temporary of type *unsinged int*. The optimizer will still manage to see through the **memcpy** and optimize away both the temporary and the copy and operate directly on the underlying data, [Live Compiler Explorer Example](https://godbolt.org/g/acjqjD):

```
// Simple operation just return the value back
int foo( unsigned int x ) { return x ; }

// Assume len is a multiple of sizeof(unsigned int) 
int bar( unsigned char *p, size_t len ) {
  int result = 0;

  for( size_t index = 0; index < len; index += sizeof(unsigned int) ) {
    unsigned int ui = 0;                                 
    std::memcpy( &ui, &p[index], sizeof(unsigned int) );

    result += foo( ui ) ;
  }

  return result;
}
```

In the example, we take a *char** **p**, assume it points to multiple chunks of **sizeof(unsigned int)** data, we type pun each chunk of data as an *unsigned int*, compute **foo()** on each chunk of type punned data and sum it into **result** and return the final value.

The assembly for the body of the loop shows the optimizer reduces the body into a direct access of the underlying *unsigned char array* as an *unsigned int*, adding it directly into **eax**:

```
add     eax, dword ptr [rdi + rcx] 
```

Same code but using **reinterpret_cast** to type pun(violates strict aliasing):

```
// Assume len is a multiple of sizeof(unsigned int) 
int bar( unsigned char *p, size_t len ) {
 int result = 0;

 for( size_t index = 0; index < len; index += sizeof(unsigned int) ) {
   unsigned int ui = *reinterpret_cast<unsigned int*>(&p[index]);

   result += foo( ui );
 }

 return result;
}
```

## C++20 and bit_cast

In C++20 we may gain **bit_cast**[14](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f14) which gives a simple and safe way to type-pun as well as being usable in a constexpr context.

The following is an example of how to use **bit_cast** to type pun a *unsigned int* to *float*, ([see it live](https://wandbox.org/permlink/i5l0g4IYuCFgLzzl)):

```
std::cout << bit_cast<float>(0x447a0000) << "\n" ; //assuming sizeof(float) == sizeof(unsigned int)
```

In the case where *To* and *From* types don't have the same size, it requires us to use an intermediate struct[15](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f15). We will use a struct containing a **sizeof( unsigned int )** character array (*assumes 4 byte unsigned int*) to be the *From* type and *unsigned int*as the *To* type.:

```
struct uint_chars {
 unsigned char arr[sizeof( unsigned int )] = {} ;  // Assume sizeof( unsigned int ) == 4
};

// Assume len is a multiple of 4 
int bar( unsigned char *p, size_t len ) {
 int result = 0;

 for( size_t index = 0; index < len; index += sizeof(unsigned int) ) {
   uint_chars f;
   std::memcpy( f.arr, &p[index], sizeof(unsigned int));
   unsigned int result = bit_cast<unsigned int>(f);

   result += foo( result );
 }

 return result ;
}
```

It is unfortunate that we need this intermediate type but that is the current contraint of **bit_cast**.

## Alignment

We have seen in previous examples violating strict aliasing rules can lead to stores being optimized away. Violating strict aliasing rules can also lead to violations of alignment requirement. Both the C and C++ standard state that objects have *alignment requirements* which restrict where objects can be allocated (*in memory*) and therefore accessed[17](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f17). C11 section *6.2.8 Alignment of objects* says:

> Complete object types have alignment requirements which place restrictions on the addresses at which objects of that type may be allocated. An alignment is an implementation-defined integer value representing the number of bytes between successive addresses at which a given object can be allocated. An object type imposes an alignment requirement on every object of that type: stricter alignment can be requested using the _Alignas keyword.

The C++17 draft standard in section *[basic.align] paragraph 1*:

> Object types have alignment requirements (6.7.1, 6.7.2) which place restrictions on the addresses at which an object of that type may be allocated. An alignment is an implementation-defined integer value representing the number of bytes between successive addresses at which a given object can be allocated. An object type imposes an alignment requirement on every object of that type; stricter alignment can be requested using the alignment specifier (10.6.2).

Both C99 and C11 are explicit that a conversion that results in a unaligned pointer is undefined behavior, section *6.3.2.3 Pointers* says:

> A pointer to an object or incomplete type may be converted to a pointer to a different object or incomplete type. If the resulting pointer is not correctly aligned57) for the pointed-to type, the behavior is undefined. ...

Although C++ is not as explict I believe this sentence from *[basic.align] paragraph 1* is sufficient:

> ... An object type imposes an alignment requirement on every object of that type; ...

### An Example

So let's assume:

- **alignof(char)** and **alignof(int)** are 1 and 4 respectively
- sizeof(int) is 4

Then type punning an array of char of size 4 as an *int* violates strict aliasing but may also violate alignment requirements if the array has an alignment of 1 or 2 bytes.

```
char arr[4] = { 0x0F, 0x0, 0x0, 0x00 }; // Could be allocated on a 1 or 2 byte boundary
int x = *reinterpret_cast<int*>(arr);   // Undefined behavior we have an unaligned pointer
```

Which could lead to reduced performance or a bus error[18](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f18) in some situations. Whereas using **alignas** to force the array to the same alignment of *int* would prevent violating alignment requirements:

```
alignas(alignof(int)) char arr[4] = { 0x0F, 0x0, 0x0, 0x00 }; 
int x = *reinterpret_cast<int*>(arr);
```

### Atomics

Another unexpected penalty to unaligned accesses is that it breaks atomics on some architectures. Atomic stores may not appear atomic to other threads on x86 if they are misaligned[7](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f7).

## Catching Strict Aliasing Violations

We don't have a lot of good tools for catching strict aliasing in C++, the tools we have will catch some cases of strict aliasing violations and some cases of misaligned loads and stores.

gcc using the flag **-fstrict-aliasing** and **-Wstrict-aliasing**[19](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f19) can catch some cases although not without false positives/negatives. For example the following cases[21](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f21) will generate a warning in gcc ([see it live](https://wandbox.org/permlink/cfckjTgwNTYHDIry)):

```
int a = 1;
short j;
float f = 1.f; // Originally not initialized but tis-kernel caught 
               // it was being accessed w/ an indeterminate value below

printf("%i\n", j = *(reinterpret_cast<short*>(&a)));
printf("%i\n", j = *(reinterpret_cast<int*>(&f)));
```

although it will not catch this additional case ([see it live](https://wandbox.org/permlink/dwd9jhy53AF7a2D0)):

```
int *p;

p=&a;
printf("%i\n", j = *(reinterpret_cast<short*>(p)));
```

Although clang allows these flags it apparently does not actually implement the warnings[20](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f20).

Another tool we have available to us is ASan[22](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f22) which can catch misaligned loads and stores. Although these are not directly strict aliasing violations they are a common result of strict aliasing violations. For example the following cases[23](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f23) will generate runtime errors when built with clang using **-fsanitize=address**

```
int *x = new int[2];               // 8 bytes: [0,7].
int *u = (int*)((char*)x + 6);     // regardless of alignment of x this will not be an aligned address
*u = 1;                            // Access to range [6-9]
printf( "%d\n", *u );              // Access to range [6-9]
```

The last tool I will recommend is C++ specific and not strictly a tool but a coding practice, don't allow C-style casts. Both gcc and clang will produce a diagnostic for C-style casts using **-Wold-style-cast**. This will force any undefined type puns to use reinterpret_cast, in general reinterpret_cast should be a flag for closer code review. It is also easiser to search your code base for reinterpret_cast to perform an audit.

For C we have all the tools already covered and we also have tis-interpreter[24](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f24), a static analyzer that exhaustively analyzes a program for a large subset of the C language. Given a C verions of the earlier example where using **-fstrict-aliasing** misses one case ([see it live](https://wandbox.org/permlink/ebLBJ17Pg7TsnIgY))

```
int a = 1;
short j;
float f = 1.0 ;

printf("%i\n", j = *((short*)&a));
printf("%i\n", j = *((int*)&f));
    
int *p; 

p=&a;
printf("%i\n", j = *((short*)p));
```

tis-interpeter is able to catch all three, the following example invokes tis-kernal as tis-interpreter (output is edited for brevity):

```
./bin/tis-kernel -sa example1.c 
...
example1.c:9:[sa] warning: The pointer (short *)(& a) has type short *. It violates strict aliasing
              rules by accessing a cell with effective type int.
...

example1.c:10:[sa] warning: The pointer (int *)(& f) has type int *. It violates strict aliasing rules by
              accessing a cell with effective type float.
              Callstack: main
...

example1.c:15:[sa] warning: The pointer (short *)p has type short *. It violates strict aliasing rules by
              accessing a cell with effective type int.
```

Finally there is [TySan](https://www.youtube.com/watch?v=vAXJeN7k32Y)[26](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#f26) which is currently in development. This sanitizer adds type checking information in a shadow memory segment and checks accesses to see if they violate aliasing rules. The tool potentially should be able to catch all aliasing violations but may have a large run-time overhead.

## Conclusion

We have learned about aliasing rules in both C and C++, what it means that the compiler expects that we follow these rules strictly and the consequences of not doing so. We learned about some tools that will help us catch some misuses of aliasing. We have seen a common use for type aliasing is type punning and how to type pun correctly.

Optimizers are slowly getting better at type based aliasing analysis and already break some code that relies on strict aliasing violations. We can expect the optimizations will only get better and will break more code we have been used to just working.

We have standard conformant methods for type punning and in release and sometimes debug builds these methods should be cost free abstractions. We have some tools for catching strict aliasing violations but for C++ they will only catch a small fraction of the cases and for C with tis-interpreter we should be able to catch most violations.

Thank you to those who provided feedback on this write-up: JF Bastien, Christopher Di Bella, Pascal Cuoq, Matt P. Dziubinski, Patrice Roy and Ólafur Waage

Of course in the end, all errors are the author's.

#### Footnotes

**1** Undefined behavior described on cppreference http://en.cppreference.com/w/cpp/language/ub [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a1) 
**2** Draft C11 standard is freely available http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a2) 
**3** Draft C++17 standard is freely available https://github.com/cplusplus/draft/raw/master/papers/n4659.pdf [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a3) 
**4** Latest C++ draft standard can be found here: http://eel.is/c++draft/ [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a4) 
**5** Understanding lvalues and rvalues in C and C++ https://eli.thegreenplace.net/2011/12/15/understanding-lvalues-and-rvalues-in-c-and-c [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a5) 
**6** Type-Based Alias Analysis http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/DDJ/2000/0010/0010d/0010d.htm [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a6) 
**7** Demonstrates torn loads for misaligned atomics https://gist.github.com/michaeljclark/31fc67fe41d233a83e9ec8e3702398e8 and tweet referencing this example https://twitter.com/corkmork/status/944421528829009925 [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a7) 
**8** Comment in gcc bug report explaining why changing int8_t and uint8_t to not be char types would be an ABI break for C++ https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66110#c13 and twitter thread discussing the issue https://twitter.com/shafikyaghmour/status/822179548825468928 [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a8) 
**9** "New” Value Terminology which explains how glvalue, xvalue and prvalue came about http://www.stroustrup.com/terminology.pdf [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a9) 
**10** Effective types and aliasing https://gustedt.wordpress.com/2016/08/17/effective-types-and-aliasing/ [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a10) 
**11** “constructing” a trivially-copyable object with memcpy https://stackoverflow.com/q/30114397/1708801 [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a11)

**12** Why does gcc and clang allow assigning an unsigned int * to int * since they are not compatible types, although they may alias https://twitter.com/shafikyaghmour/status/957702383810658304 and https://gcc.gnu.org/ml/gcc/2003-10/msg00184.html [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a12) 
**13** Unions and memcpy and type punning https://stackoverflow.com/q/25664848/1708801 [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a13) 
**14** Revision two of the bit_cast<> proposal http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0476r2.html [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a14) 
**15** How to use bit_cast to type pun a unsigned char array https://gist.github.com/shafik/a956a17d00024b32b35634eeba1eb49e [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a15) 
**16** bit_cast implementation of pop() https://godbolt.org/g/bXBie7 [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a16) 
**17** Unaligned access https://en.wikipedia.org/wiki/Bus_error#Unaligned_access [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a17) 
**18** A bug story: data alignment on x86 http://pzemtsov.github.io/2016/11/06/bug-story-alignment-on-x86.html [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a18) 
**19** gcc documentation for -Wstrict-aliasing https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html#index-Wstrict-aliasing [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a19) 
**20** Comments indicating clang does not implement -Wstrict-aliasing https://github.com/llvm-mirror/clang/blob/master/test/Misc/warning-flags-tree.c [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a20) 
**21** Stack Overflow questions examples came from https://stackoverflow.com/q/25117826/1708801 [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a21) 
**22** ASan documentation https://clang.llvm.org/docs/AddressSanitizer.html [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a22) 
**23** The unaligned access example take from the Address Sanitizer Algorithm wiki https://github.com/google/sanitizers/wiki/AddressSanitizerAlgorithm#unaligned-accesses [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a23) 
**24** TrustInSoft tis-interpreter https://trust-in-soft.com/tis-interpreter/ , strict aliasing checks can be run by building tis-kernel https://github.com/TrustInSoft/tis-kernel [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a24) 
**25** Detecting Strict Aliasing Violations in the Wild https://trust-in-soft.com/wp-content/uploads/2017/01/vmcai.pdf a paper that covers dos and don't w.r.t to aliasing in C [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a25) 
**26** TySan patches, clang: https://reviews.llvm.org/D32199 runtime: https://reviews.llvm.org/D32197 llvm: https://reviews.llvm.org/D32198 [↩](https://gist.github.com/shafik/848ae25ee209f698763cffee272a58f8#a26)