# Array index

在下面文章，中也讨论了这个问题:

1、pvs-studio [About size_t and ptrdiff_t](https://pvs-studio.com/en/a/0050/)

## CppCoreGuidelines [ES.107: Don't use `unsigned` for subscripts, prefer `gsl::index`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#es107-dont-use-unsigned-for-subscripts-prefer-gslindex)

### Reason

To avoid signed/unsigned confusion. To enable better optimization. To enable better error detection. To avoid the pitfalls with `auto` and `int`.

### Example, bad

```c++
vector<int> vec = /*...*/;

for (int i = 0; i < vec.size(); i += 2)                    // might not be big enough
    cout << vec[i] << '\n';
for (unsigned i = 0; i < vec.size(); i += 2)               // risk wraparound
    cout << vec[i] << '\n';
for (auto i = 0; i < vec.size(); i += 2)                   // might not be big enough
    cout << vec[i] << '\n';
for (vector<int>::size_type i = 0; i < vec.size(); i += 2) // verbose
    cout << vec[i] << '\n';
for (auto i = vec.size()-1; i >= 0; i -= 2)                // bug
    cout << vec[i] << '\n';
for (int i = vec.size()-1; i >= 0; i -= 2)                 // might not be big enough
    cout << vec[i] << '\n';
```

### Example, good

```c++
vector<int> vec = /*...*/;

for (gsl::index i = 0; i < vec.size(); i += 2)             // ok
    cout << vec[i] << '\n';
for (gsl::index i = vec.size()-1; i >= 0; i -= 2)          // ok
    cout << vec[i] << '\n';
```

### Note

The built-in array uses **signed subscripts**. The standard-library containers use **unsigned subscripts**. Thus, no perfect and fully compatible solution is possible (unless and until the standard-library containers change to use signed subscripts someday in the future). Given the known problems with unsigned and signed/unsigned mixtures, better stick to (signed) integers of a sufficient size, which is guaranteed by `gsl::index`.

### Example

```c++
template<typename T>
struct My_container {
public:
    // ...
    T& operator[](gsl::index i);    // not unsigned
    // ...
};
```

### Example

```
??? demonstrate improved code generation and potential for error detection ???
```

### Alternatives

Alternatives for users

- use algorithms
- use range-for
- use iterators/pointers

### Enforcement

- Very tricky as long as the standard-library containers get it wrong.
- (To avoid noise) Do not flag on a mixed signed/unsigned comparison where one of the arguments is `sizeof` or a call to container `.size()` and the other is `ptrdiff_t`.