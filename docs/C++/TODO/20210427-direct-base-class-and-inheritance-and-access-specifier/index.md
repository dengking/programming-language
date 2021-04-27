## cppreference [Derived classes](https://en.cppreference.com/w/cpp/language/derived_class)

>  C++ 的 inheritance是支持指定 access-specifier的，参见

```C++
attr(optional) access-specifier(optional) virtual-specifier(optional) class-or-decltype		
```

### Default access 

If *access-specifier* is omitted, it defaults to `public` for classes declared with *class-key* `struct` and to `private` for classes declared with *class-key* `class`.



### Public inheritance

### Protected inheritance

### Private inheritance

Private inheritance is commonly used in policy-based design, since policies are usually empty classes, and using them as bases both enables static polymorphism and leverages [empty-base optimization](https://en.cppreference.com/w/cpp/language/ebo)



## 不可直接访问的base class

### cplusplus [error: 'class1' is not a direct base of 'class2'](http://www.cplusplus.com/forum/general/48524/)



### stackoverflow [Warning: direct base class inaccessible in derived due to ambiguity; is this serious?](https://stackoverflow.com/questions/30246098/warning-direct-base-class-inaccessible-in-derived-due-to-ambiguity-is-this-ser)