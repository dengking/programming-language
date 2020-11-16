# `enum` as key



## `enum` class as key



### stackoverflow [Can't use enum class as unordered_map key](https://stackoverflow.com/questions/18837857/cant-use-enum-class-as-unordered-map-key)

Q

I have a class containing an enum class.

```cpp
class Shader {
public:
    enum class Type {
        Vertex   = GL_VERTEX_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };
    //...
```

Then, when I implement the following code in another class...

```cpp
std::unordered_map<Shader::Type, Shader> shaders;
```

...I get a compile error.

```cpp
...usr/lib/c++/v1/type_traits:770:38: 
Implicit instantiation of undefined template 'std::__1::hash<Shader::Type>'
```

What is causing the error here?

[A](https://stackoverflow.com/a/24847480)

I use a functor object to calculate hash of `enum class`:

```cpp
struct EnumClassHash
{
    template <typename T>
    std::size_t operator()(T t) const
    {
        return static_cast<std::size_t>(t);
    }
};
```

Now you can use it as 3rd template-parameter of `std::unordered_map`:

```cpp
enum class MyEnum {};

std::unordered_map<MyEnum, int, EnumClassHash> myMap;
```

So you don't need to provide a specialization of `std::hash`, the **template argument deduction** does the job. Furthermore, you can use the word `using` and make your own `unordered_map` that use `std::hash` or `EnumClassHash` depending on the `Key` type:

```cpp
template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;

template <typename Key, typename T>
using MyUnorderedMap = std::unordered_map<Key, T, HashType<Key>>;
```

Now you can use `MyUnorderedMap` with `enum class` or another type:

```cpp
MyUnorderedMap<int, int> myMap2;
MyUnorderedMap<MyEnum, int> myMap3;
```

Theoretically, `HashType` could use `std::underlying_type` and then the `EnumClassHash` will not be necessary. That could be something like this, **but I haven't tried yet**:

```cpp
template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, std::hash<std::underlying_type<Key>::type>, std::hash<Key>>::type;
```

If using `std::underlying_type` works, could be a very good proposal for the standard.



### cplusplus [enum class as map key](http://www.cplusplus.com/forum/general/74363/)





