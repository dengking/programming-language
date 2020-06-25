# Barton–Nackman trick



## More C++ Idioms/[Barton-Nackman trick](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Barton-Nackman_trick)



### Intent

Support overloaded operators without relying on namespaces or function template overload resolution.



### Motivation

```c++
template<typename T>
class List {
   // ...
};
```



```c++
template<typename T>
bool operator==(List<T> const & lft, List<T> const & rgt) {
   //...
}
```



### Solution and Sample Code

```c++
template<typename T>
class List {
 public:
    friend bool operator==(const List<T> & lft,
                           const List<T> & rgt) {
        // ...
    }
};
```



```c++
template<typename T>
class EqualityComparable {
public:
    friend bool operator==(const T & lft, const T & rgt) { return lft.equalTo(rgt); }
    friend bool operator!=(const T & lft, const T & rgt) { return !lft.equalTo(rgt); }
};

class ValueType :
    private EqualityComparable<ValueType> {
 public:
    bool equalTo(const ValueType & other) const;
};
```



### Known Uses

[Boost.Operators library](http://www.boost.org/doc/libs/1_50_0/libs/utility/operators.htm)



### Related Idioms

[Curiously Recurring Template Pattern](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Curiously_Recurring_Template_Pattern)



## Wikipedia [Barton-Nackman trick](https://en.wikipedia.org/wiki/Barton%E2%80%93Nackman_trick)





http://www.open-std.org/jtc1/sc22/wg21/docs/papers/1995/N0777.pdf