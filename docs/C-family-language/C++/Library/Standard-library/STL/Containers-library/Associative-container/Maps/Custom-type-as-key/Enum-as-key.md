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

> NOTE: 完整测试程序如下:
>
> ```C++
> #include <unordered_map>
> /**
>  * @brief 着色器
>  *
>  */
> class Shader
> {
> public:
> 	enum class Type
> 	{
> 		Vertex = 1, Geometry = 2, Fragment = 3
> 	};
> 	//...
> };
> 
> int main()
> {
> 	std::unordered_map<Shader::Type, Shader> shaders;
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 编译报错如下:
>
> ```C++
> /usr/include/c++/4.8.2/bits/hashtable_policy.h:1070:12: error: invalid use of incomplete type ‘struct std::hash<Shader::Type>’
>      struct _Hash_code_base<_Key, _Value, _ExtractKey, _H1, _H2,
> 
> ```
>
> 

[A](https://stackoverflow.com/a/24847480)

I use a **functor object** to calculate hash of `enum class`:

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

> NOTE: 完整测试程序如下:
>
> ```C++
> #include <unordered_map>
> #include <iostream>
> 
> /**
>  * @brief 着色器
>  *
>  */
> class Shader
> {
> public:
> 	enum class Type
> 	{
> 		Vertex = 1, Geometry = 2, Fragment = 3
> 	};
> 	//...
> };
> 
> struct EnumClassHash
> {
> 	template<typename T>
> 	std::size_t operator()(T t) const
> 	{
> 		std::cout << __FILE__ << " " << __LINE__ << " " << std::endl;
> 		return static_cast<std::size_t>(t);
> 	}
> };
> 
> int main()
> {
> 	std::unordered_map<Shader::Type, Shader, EnumClassHash> shaders;
> 	shaders[Shader::Type::Vertex] = Shader();
> 	shaders[Shader::Type::Geometry] = Shader();
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 输出如下:
>
> ```
> test.cpp 23 
> test.cpp 23 
> ```
>
> 可以看到，`EnumClassHash`的`operator()(T t)`会被调用。
>
> 上述例子将`EnumClassHash`来作为hash function，可以看到，`EnumClassHash`仅仅将enum的值作为hash value，显然这不是一个好的hash function。

So you don't need to provide a **specialization** of `std::hash`, the **template argument deduction** does the job. 

> NOTE: 这段话的意思是: 对于function template，C++ compiler会自动进行template argument deduction。

Furthermore, you can use the word `using` and make your own `unordered_map` that use `std::hash` or `EnumClassHash` depending on the `Key` type:

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

> NOTE:  完整测试程序如下:
>
> ```C++
> #include <unordered_map>
> #include <iostream>
> #include <type_traits>
> 
> /**
>  * @brief 着色器
>  *
>  */
> class Shader
> {
> public:
> 	enum class Type
> 	{
> 		Vertex = 1, Geometry = 2, Fragment = 3
> 	};
> 	//...
> };
> 
> struct EnumClassHash
> {
> 	template<typename T>
> 	std::size_t operator()(T t) const
> 	{
> 		std::cout << __FILE__ << " " << __LINE__ << " " << std::endl;
> 		return static_cast<std::size_t>(t);
> 	}
> };
> 
> template<typename Key>
> using HashType = typename std::conditional<std::is_enum<Key>::value, EnumClassHash, std::hash<Key>>::type;
> 
> template<typename Key, typename T>
> using MyUnorderedMap = std::unordered_map<Key, T, HashType<Key>>;
> 
> int main()
> {
> 	MyUnorderedMap<int, int> myMap2;
> 	MyUnorderedMap<Shader::Type, int> myMap3;
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 



Theoretically, `HashType` could use `std::underlying_type` and then the `EnumClassHash` will not be necessary. That could be something like this, **but I haven't tried yet**:

```cpp
template <typename Key>
using HashType = typename std::conditional<std::is_enum<Key>::value, std::hash<std::underlying_type<Key>::type>, std::hash<Key>>::type;
```

If using `std::underlying_type` works, could be a very good proposal for the standard.

> NOTE: 完整测试程序如下:
>
> ```C++
> #include <unordered_map>
> #include <iostream>
> #include <type_traits>
> 
> /**
>  * @brief 着色器
>  *
>  */
> class Shader
> {
> public:
> 	enum class Type
> 	{
> 		Vertex = 1, Geometry = 2, Fragment = 3
> 	};
> 	//...
> };
> 
> struct EnumClassHash
> {
> 	template<typename T>
> 	std::size_t operator()(T t) const
> 	{
> 		std::cout << __FILE__ << " " << __LINE__ << " " << std::endl;
> 		return static_cast<std::size_t>(t);
> 	}
> };
> 
> template<typename Key>
> using HashType = typename std::conditional<std::is_enum<Key>::value, std::hash<std::underlying_type<Key>::type>, std::hash<Key>>::type;
> 
> template<typename Key, typename T>
> using MyUnorderedMap = std::unordered_map<Key, T, HashType<Key>>;
> 
> int main()
> {
> 	MyUnorderedMap<int, int> myMap2;
> 	MyUnorderedMap<Shader::Type, int> myMap3;
> }
> // g++ --std=c++11 test.cpp
> 
> ```
>
> 上述程序编译报错如下:
>
> ```C++
> est.cpp:30:111: error: type/value mismatch at argument 1 in template parameter list for ‘template<class _Tp> struct std::hash’
>  using HashType = typename std::conditional<std::is_enum<Key>::value, std::hash<std::underlying_type<Key>::type>, std::hash<Key>>::type;
> ```



[A](https://stackoverflow.com/a/29618545)

This was considered a defect in the standard, and was fixed in C++14: http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#2148

This is fixed in the version of libstdc++ shipping with gcc as of 6.1: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=60970.

It was fixed in clang's libc++ in 2013: http://lists.cs.uiuc.edu/pipermail/cfe-commits/Week-of-Mon-20130902/087778.html



### cplusplus [enum class as map key](http://www.cplusplus.com/forum/general/74363/)





