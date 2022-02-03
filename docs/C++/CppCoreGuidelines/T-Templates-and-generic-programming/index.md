# CppCoreGuidelines [T: Templates and generic programming](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t-templates-and-generic-programming)



## Template and hierarchy rule summary:

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

## Other template rules summary:

### [T.150: Check that a class matches a concept using `static_assert`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t150-check-that-a-class-matches-a-concept-using-static_assert)

> NOTE: 
>
> 1、参见 `static_assert` 章节

