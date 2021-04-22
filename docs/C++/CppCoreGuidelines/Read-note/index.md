### [I.22: Avoid complex initialization of global objects](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i22-avoid-complex-initialization-of-global-objects)

> NOTE: 
>
> 1、这其实就是Static-Initialization-Order-Fiasco，参见 `Static-storage-duration` 章节



### [I.4: Make interfaces precisely and strongly typed](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i4-make-interfaces-precisely-and-strongly-typed)

> NOTE: 
>
> 1、参见 `Strong-type` 章节



### [T.83: Do not declare a member function template virtual](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t83-do-not-declare-a-member-function-template-virtual)

#### Reason

C++ does not support that. If it did, vtbls could not be generated until link time. And in general, implementations must deal with dynamic linking.

> NOTE: 
>
> 1、当前，virtual table是compile time生成的；如果支持template virtual function，那么virtual table需要直到link time才能够生成，并且还需要支持dynamic linking。
>
> 2、参见 `Template-and-virtual` 章节

#### Alternative

Double dispatch, visitors, calculate which function to call

### [T.84: Use a non-template core implementation to provide an ABI-stable interface](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t84-use-a-non-template-core-implementation-to-provide-an-abi-stable-interface)

> NOTE: 
>
> 1、这其实就是在 `Type-Erasure` 章节中，总结的: "OOP interface + template implementation"

#### Example

It could be a base class:

```c++
struct Link_base {   // stable
    Link_base* suc;
    Link_base* pre;
};

template<typename T>   // templated wrapper to add type safety
struct Link : Link_base {
    T val;
};

struct List_base {
    Link_base* first;   // first element (if any)
    int sz;             // number of elements
    void add_front(Link_base* p);
    // ...
};

template<typename T>
class List : List_base {
public:
    void put_front(const T& e) { add_front(new Link<T>{e}); }   // implicit cast to Link_base
    T& front() { static_cast<Link<T>*>(first).val; }   // explicit cast back to Link<T>
    // ...
};C

List<int> li;
List<string> ls;
```



### [T.150: Check that a class matches a concept using `static_assert`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t150-check-that-a-class-matches-a-concept-using-static_assert)

> NOTE: 
>
> 1、参见 `static_assert` 章节

### [SF.22: Use an unnamed (anonymous) namespace for all internal/non-exported entities](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#sf22-use-an-unnamed-anonymous-namespace-for-all-internalnon-exported-entities)



### [ES.20: Always initialize an object](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es20-always-initialize-an-object)

> NOTE: 
>
> 1、`Default-initialization`:
>
> > 对于built-in type，如果不对它进行initialization的话，那么它就是Indeterminate value，显然这是比较危险的；我们最好遵循
> >
> > CppCoreGuidelines [ES.20: Always initialize an object](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es20-always-initialize-an-object)
>
> 

## Arithmetic

### [ES.100: Don't mix signed and unsigned arithmetic](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es100-dont-mix-signed-and-unsigned-arithmetic)

> NOTE: 
>
> 1、在 pvs-studio [About size_t and ptrdiff_t](https://pvs-studio.com/en/a/0050/) 中，有非常好的例子



### [ES.107: Don't use `unsigned` for subscripts, prefer `gsl::index`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es107-dont-use-unsigned-for-subscripts-prefer-gslindex)

> NOTE: 
>
> 1、参见 `Array-index` 章节





### [C.50: Use a factory function if you need "virtual behavior" during initialization](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c50-use-a-factory-function-if-you-need-virtual-behavior-during-initialization)

> NOTE:
>
> 1、其实就是`Calling-Virtual-During-Initialization-idiom`

### [C.52: Use inheriting constructors to import constructors into a derived class that does not need further explicit initialization](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c52-use-inheriting-constructors-to-import-constructors-into-a-derived-class-that-does-not-need-further-explicit-initialization)

> NOTE: 



## C.copy: Copy and move