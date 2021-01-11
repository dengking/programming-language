

# `c++` `isinstanceof` `issubclass`

http://www.cplusplus.com/forum/general/50090/



https://www.tutorialspoint.com/cplusplus-equivalent-of-instanceof



## stackoverflow [C++ equivalent of java's instanceof](https://stackoverflow.com/questions/500493/c-equivalent-of-javas-instanceof)

[A](https://stackoverflow.com/a/500495)

Try using:

```C++
if(NewType* v = dynamic_cast<NewType*>(old)) {
   // old was safely casted to NewType
   v->doSomething();
}
```

This requires your compiler to have rtti support enabled.

EDIT: I've had some good comments on this answer!

Every time you need to use a `dynamic_cast` (or `instanceof`) you'd better ask yourself whether it's a necessary thing. It's generally a sign of poor design.

Typical workarounds is putting the special behaviour for the class you are checking for into a virtual function on the base class or perhaps introducing something like a [visitor](http://en.wikipedia.org/wiki/Visitor_pattern) where you can introduce specific behaviour for subclasses without changing the interface (except for adding the visitor acceptance interface of course).

As pointed out `dynamic_cast` doesn't come for free. A simple and consistently performing hack that handles most (but not all cases) is basically adding an enum representing all the possible types your class can have and check whether you got the right one.

```C++
if(old->getType() == BOX) {
   Box* box = static_cast<Box*>(old);
   // Do something box specific
}
```

This is not good oo design, but it can be a workaround and its cost is more or less only a virtual function call. It also works regardless of RTTI is enabled or not.

Note that this approach doesn't support multiple levels of inheritance so if you're not careful you might end with code looking like this:

```c++
// Here we have a SpecialBox class that inherits Box, since it has its own type
// we must check for both BOX or SPECIAL_BOX
if(old->getType() == BOX || old->getType() == SPECIAL_BOX) {
   Box* box = static_cast<Box*>(old);
   // Do something box specific
}
```

[A](https://stackoverflow.com/a/41086124)

`dynamic_cast` is known to be inefficient. It traverses up the inheritance hierarchy, and *it is* the only solution if you have multiple levels of inheritance, and need to check if an object is an instance of any one of the types in its type hierarchy.

But if a more limited form of `instanceof` that only checks if an object is exactly the type you specify, suffices for your needs, the function below would be a lot more efficient:

```C++
template<typename T, typename K>
inline bool isType(const K &k) {
    return typeid(T).hash_code() == typeid(k).hash_code();
}
```

Here's an example of how you'd invoke the function above:

```C++
DerivedA k;
Base *p = &k;

cout << boolalpha << isType<DerivedA>(*p) << endl;  // true
cout << boolalpha << isType<DerivedB>(*p) << endl;  // false
```

You'd specify template type `A` (as the type you're checking for), and pass in the object you want to test as the argument (from which template type `K` would be inferred).