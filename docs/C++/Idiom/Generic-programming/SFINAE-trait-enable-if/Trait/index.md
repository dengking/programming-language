# C++ traits

## accu [An introduction to C++ Traits](https://accu.org/index.php/journals/442)

It is not uncommon to see different pieces of code that have basically the same structure, but contain variation in the details. Ideally we would be able to reuse the structure, and factor out the variations. In 'C' this might be done by using **function pointers**, as in the C Standard Library `qsort` function or in C++ by using **virtual functions**. Unfortunately this differed to **runtime** what is known at **compile time**, and became with a runtime overhead.

C++ introduces generic programming, with `template`s, eliminating the need for **runtime binding**, but at first glance this still looks like a compromise, after all, the same algorithm will not work optimally with every data structure. Sorting a linked list is different to sorting an array. Sorted data can be searched much faster than unsorted data.

The C++ traits technique provides an answer.

> Think of a **trait** as a small object whose main purpose is to carry information used by another object or algorithm to determine "policy" or "implementation details". - Bjarne Stroustrup

### [`std::numeric_limits`](https://en.cppreference.com/w/cpp/types/numeric_limits)

Both C and `C++` programmers should be familiar with [`limits.h`](https://code.woboq.org/gcc/libstdc++-v3/include/std/limits.html) , and `float.h`, which are used to determine the various properties of the integer and floating point types.

Most C++ programmers are familiar with [`std::numeric_limits`](https://en.cppreference.com/w/cpp/types/numeric_limits) , which at first glance simply provides the same service, implemented differently. By taking a closer look at `numeric_limits` we uncover the first advantage of traits, a **consistent interface**.

> NOTE: 千万不要小看上面这段话中的**consistent interface**的价值，如果能够保证**consistent interface**，那么当我们修改底层代码的时候，caller是无需修改的， 但是如果interface是unconsistent的，那么caller是需要修改的。此处的consistent interface，是一种抽象，它能够带来Polymorphism，参见`Theory\Programming-paradigm\Object-oriented-programming\Polymorphism\Polymorphism.md`的《Template and polymorphism》段。
>
> 在阅读[boost graph](https://www.boost.org/doc/libs/release/libs/graph/doc/)的下面这段话的时候，我想到：trait提供了一层abstraction，提供了获取信息的interface。
>
> > Note that different graph classes can have different associated vertex iterator types, which is why we need the `graph_traits` class. Given some graph type, the `graph_traits` class will provide access to the `vertex_iterator` type.

Using `float.h`, and `limits.h`, you have to remember the type prefix and the trait, for example, `DBL_MAX` contains the "maximum value" trait for the double data type. By using a traits class such as `numeric_limits` the type becomes part of the name, so that the maximum value for a double becomes `numeric_limits< double >::max()`, more to the point, you don't need to know which type you need to use. For example, take this simple template function (adapted from [[Veldhuizen](https://accu.org/index.php/journals/442#Veldhuizen)]), which returns the largest value in an array:

```c++
template< class T > 
T findMax(const T const * data, 
         const size_t const numItems) { 
  // Obtain the minimum value for type T 
  T largest = 
      std::numeric_limits< T >::min(); 
  for(unsigned int i=0; i<numItems; ++i) 
  	if (data[i] > largest) 
  		largest = data[i]; 
  return largest; 
} 
```

> NOTE : 上述代码中的  
>
> ```C++
> // Obtain the minimum value for type T 
> T largest = std::numeric_limits< T >::min(); 
> ```
>
> 即为trait的使用。



Note the use of `numeric_limits`. As you can see, where as with the C style `limits.h` idiom, where you must know the type, **with the `C++` traits idiom, only the compiler needs to know the type**. Not only that, but `numeric_limits`, as with most traits, can be extended to include your own custom types (such as a fixed point, or arbitrary precision arithmetic classes) simply by creating a **specialization** of the template.

> NOTE: 
>
> trait不仅能够规避程序员手写而产生的错误，并且还能够减少编码量
>
> trait还带来了可扩展性

> NOTE: `numeric_limits`是实现：[libstdc++-v3](https://code.woboq.org/gcc/libstdc++-v3/)/[include](https://code.woboq.org/gcc/libstdc++-v3/include/)/[std](https://code.woboq.org/gcc/libstdc++-v3/include/std/)/[limits](https://code.woboq.org/gcc/libstdc++-v3/include/std/limits.html)

### `is_void`

But I'd like to move away from `numeric_limits`, its just an example of traits in action, and I'd like to take you through creating traits classes of your own.

First lets look at one of the simplest traits classes you can get (from [boost.org](http://www.boost.org/)) and that's the `is_void` [[boost](https://accu.org/index.php/journals/442#boost)] trait.

First, **a generic template** is defined that implements the **default behaviour**. In this case, all but one type is `void`, so `is_void::value` should be `false`, so we start with:

```cpp
template< typename T > 
struct is_void{ 
  static const bool value = false;
};
```

Add to that a **specialization** for `void`:

```c++
template<> 
struct is_void< void >{ 
  static const bool value = true; 
};
```

And we have a complete traits type that can be used to detect if any given type, passed in as a **template parameter**, is `void`. Not the most useful piece of code on its own, but definitely a useful demonstration of the technique.

### `is_pointer`

Now, while **fully specialized templates** are useful and in my experience, the most common sort of trait class specialization, I think that it is worth quickly looking at **partial specialization**, in this case, `boost::is_pointer` [[boost](https://accu.org/index.php/journals/442#boost)]. Again, a default template class is defined:

```c++
template< typename T > 
struct is_pointer{ 
  static const bool value = false; 
};
```

And a **partial specialization** for all pointer types is added:

```c++
template< typename T > 
struct is_pointer< T* >{ 
  static const bool value = true; 
};
```

### `supports_optimized_implementation`

So, having got this far, how can this technique be used to solve the lowest common denominator(共同特征) problem? How can it be used to **select** an appropriate algorithm at **compile time**? This is best demonstrated with an example.

First a default traits class is created, for this example we will call it `supports_optimized_implementation`, and, other than the name, it will be the same as the `is_void` example. 

```c++
template< typename T > 
struct supports_optimised_implementation{ 
	static const bool value = false; 
};
```

Next the default algorithm is implemented, inside a templated `algorithm_selector`, in this example the `algorithm_selector` template is parameterized with a `bool`, but in situations where a number of algorithms could be appropriate, it could just as easily be parameterized with an `int`, or an `enum`. In this case `true` will mean "use optimized algorithm in object".

```c++
template< bool b > 
struct algorithm_selector { 
  template< typename T > 
  static void implementation( T& object ) 
  { 
//implement the alorithm operating on "object" here 
  } 
};
```

Next a specialization of `algorithm_selector` is added which, in this case, passes the responsibility for implementing the algorithm back to the author of the object being operated on, but could well implement a second version of the operation itself.

```c++
template<> 
struct algorithm_selector< true > { 
  template< typename T > 
  static void implementation( T& object )   { 
    object.optimised_implementation(); 
  } 
};
```

Then we write the generic function that the end user of your algorithm will call, note that it in turn calls `algorithm_selector`, parameterized using our `supports_optimised_implementation` traits class:

```c++
template< typename T > 
void algorithm( T& object ) { 
  algorithm_selector< supports_optimised_implementation< T >::value >::implementation(object); 
}
```

Now all that's left to do is test it against a class that doesn't support the feature ( `class ObjectA{};` ), and a class that does:

```c++
class ObjectB { 
public: 
  void optimised_implementation() { 
//... 
  } 
};
```

specialization of `supports_optimised_implementation` trait for `ObjectB`

```c++
template<> 
struct supports_optimised_implementation< ObjectB > { 
  static const bool value = true; 
};
```

Finally, instantiate the templates:

```c++
int main(int argc, char* argv[]) { 
  ObjectA a; 
  algorithm( a ); 
// calls default implementation 
  ObjectB b; 
  algorithm( b ); 
// calls 
// ObjectB::optimised_implementation(); 
  return 0; 
}
```

And that's it. Hopefully you can now "wow" your friends and colleague with your in-depth understanding of the c++ traits concept. :)

#### 完整可运行代码

> NOTE: 下面是完整可运行代码
```c++
#include <iostream>
template< typename T > 
struct supports_optimised_implementation{ 
	static const bool value = false; 
};

template< bool b > 
struct algorithm_selector { 
template< typename T > 
static void implementation( T& object ) 
{ 
//implement the alorithm operating on "object" here 
   std::cout<<"普通实现"<<std::endl;
} 
};

template<> 
struct algorithm_selector< true > { 
template< typename T > 
static void implementation( T& object )   { 
 object.optimised_implementation(); 
} 
};

template< typename T > 
void algorithm( T& object ) { 
algorithm_selector< supports_optimised_implementation< T >::value >::implementation(object); 
}

class ObjectA {
 
};
class ObjectB { 
public: 
void optimised_implementation() { 
//... 
   std::cout<<"优化实现"<<std::endl;
} 
};

template<> 
struct supports_optimised_implementation< ObjectB > { 
static const bool value = true; 
};

int main(int argc, char* argv[]) { 
ObjectA a; 
algorithm( a ); 
// calls default implementation 
ObjectB b; 
algorithm( b ); 
// calls 
// ObjectB::optimised_implementation(); 
return 0; 
}
```
> 编译指令如下：`g++ test.cpp`
>
> 关于上述代码有几点是需要特别强调的：
>
> trait class的`value`是compile-time constant



### Notes

It should be noted that the examples in this article require a cutting edge, standard compliant, compiler. For example, MSVC++ 6.0 does not support static constants, and will balk on:

```c++
static const bool value = false;
```

This particular problem can be worked around by using an `enum` in its place, i.e:

```c++
enum { value = false }; 
```









## bogotobogo [Traits - A Template Specialization](https://www.bogotobogo.com/cplusplus/template_specialization_traits.php)

### `numeric_limits`

Suppose we want to get max values for **int** or **double**, we do this:

```c++
#include <iostream>
#include <climits>
using namespace std;

int main()
{
	cout << "max values\n";
	cout << "int: " << INT_MAX << endl;
	cout << "double: " << DBL_MAX << endl;

	return 0;
}
```

And get the following output:

```
max values
int: 2147483647
double: 1.79769e+308
```

Then, how about **float** or **unsigned int**?

If we do not remember the name for float, we have to look it up from somewhere to get the max value.

How about this?

```c++
#include <iostream>
// #include <climits>
#include <limits>
using namespace std;

int main()
{
	cout << "max values\n";
	cout << "int: " << numeric_limits<int>::max() << endl;
	cout << "double: " << numeric_limits<double>::max() << endl;
	cout << "float: " << numeric_limits<float>::max() << endl;
	cout << "unsigned int: " << numeric_limits<unsigned int>::max() << endl;

	return 0;
}
```

Output:

```
max values
int: 2147483647
double: 1.79769e+308
float: 3.40282e+038
unsigned int: 4294967295
```

We don't have to remember the name for our query, only the compiler needs to know the type.

In the code, we used the header `<limits>` instead of `<climits>`, and the query was, `numeric_limits<type>::max()`.

What happened?

The **numeric_limits** template class replaced/supplemented the ordinary preprocessor constants of C (`<climits>` or `<limits.h>`).

### `numeric_limits` traits class

> NOTE: 这篇说明了`numeric_limits`的实现

Actually, in the `limits.h`, for example, `DBL_MAX` contains the "maximum value" **trait**(特性) for the double data **type**. However, by using a **traits class** such as **numeric_limits**, the **type ** becomes part of the name, so that the maximum value for a double becomes `numeric_limits<double>::max()`, and also, we don't need to know the type.

Usually, we use templates to implement something once for any type. But we can also use templates to provide a **common interface** that is implemented for each type. We do this by providing **specialization of a general template**.

In this tutorial, we'll see the **numeric_limits** is the typical example of specialization:

Here is a general template that provides the default numeric values for any type:

```c++
namespace std
{
	/* general numeric limits as default for any type */
	template <typename T>
	class numeric_limits
	{
	public:
		// no specialization for numeric limit exists
		static const bool is_specialized = false;
		...
	};
}
```

The general template of the numeric limits shown above is telling that there are no numeric limits available for type **T** by setting the member **is_specialized** to **false**.

We can write a specialized version of the template as below:

```c++
namespace std
{
	/* setting numeric limits for int type */
	template <>
	class numeric_limits<int>
	{
	public:
		// Now we have a specialization for numeric limit for int.
		// It does exists.
		static const bool is_specialized = true;
		static int min() throw() {
			return -2147483648;
		}
		static int max() throw() {
			return 2147483647;
		}
		static const int digits = 31;
		...
	};
}
```

The general **numeric_limits** template and its **standard specialization** are provided in the header file `<limits>`. The specialization are provided for any fundamental type that can represent numeric values: `bool`, `char`, `signed char`, `unsigned char`, `wchar_t`, `short`, `unsigned short`, `int`, `unsigned int`, long, unsigned long, float, double, and long double. They can be supplemented easily for user-defined numeric types.

```c++
#include <iostream>
#include <limits>
using namespace std;

int main()
{
	cout << "is_signed(char): " 
		<< numeric_limits<char>::is_signed << endl;
	cout << "is_specialized(long double): "
		<< numeric_limits<long double>::is_specialized << endl;
	cout << "is_specialized(std::string): "
		<< numeric_limits<std::string>::is_specialized << endl;

	return 0;
}
```

If we run the code, the **std::string** gives **false** for **is_specialized** as expected:

```
is_signed(char): 1
is_specialized(long double): 1
is_specialized(std::string): 0
```



## What is Traits?



### boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html)中关于trait的定义

在boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html)中关于trait的定义，我觉得是比较好的：

> A traits class provides a way of associating information with a compile-time entity (a type, integral constant, or address). 



So, what is **traits**?

Traits gives us additional information other than just the type. More exactly, we can get some information **about a type**.

> That's what traits let you do: they allow you to get information about a type during compilation  ...
>
> The fact that traits must work with built-in types means that things like nesting information inside types won't do, because there's no way to nest information inside pointers. The traits information for a type, then must be **external** to the type. The standard technique is to put it into a template and cone or more specializations of that template.
>
> ...
>
> By convention, traits are always implemented as structs. Another convention is that the structs used to implement traits are knows as trait **classes**.
>
> -Scott Meyers #Item 47: "Use traits classes for information about types" of his book Effective C++ 55 Specific Ways to Improve Your Programs and Designs.

Traits of `std::iterator`, please visit [Templates](http://www.bogotobogo.com/cplusplus/templates.php#restrictionstemplateparameter).

For template specialization, please visit [Template Specialization](http://www.bogotobogo.com/cplusplus/template_specialization_function_class.php).

下面总结了trait的一些特征:

### Trait is a metafunction 

Trait是典型的metafunction(参见`C++\Idiom\Templates-and-generic-programming\Metafunction`)，trait提供了compile time query static info的consistent interface。

关于这一点，在下面文章中有说明: 

1、在文章galowicz [What is a Type Trait?](https://blog.galowicz.de/2016/02/18/what_is_a_type_trait/)中，对trait的介绍也非常好:

> They are a **meta programming technique** which appears to use types and their sub types like **functions** at compile time, to control what the compiler actually compiles.

正如上面这段话中所言，trait是一种meta programming technique，上述 "**functions** at compile time"，其实就是metafunction 。



### Trait is a consistent interface and is an kind of abstraction

这是在 accu [An introduction to C++ Traits](https://accu.org/index.php/journals/442) 中提出的，我觉得总结地非常好。

### Traits is non intrusive

这是在 boost [Generic Programming Techniques](https://www.boost.org/community/generic_programming.html) # Trait 段中提出的，我觉得非常好，它说明了trait的特征。



## Implementation of trait

### Primary class template and specialization  

在[An introduction to C++ Traits](https://accu.org/index.php/journals/442)有这样的一段话：

> First, **a generic template** is defined that implements the **default behaviour**.

trait class往往会定义一个template class（primary class template ），然后提供这个template的各种specification(可能是full specialization、partial specialization)，显然在primary template class中需要提供trait的一种默认实现/默认值。



关于Template specialization，参见：

1、cppreference [explicit (full) template specialization](https://en.cppreference.com/w/cpp/language/template_specialization)

2、cppreference [partial template specialization](https://en.cppreference.com/w/cpp/language/partial_specialization)



### Trait的information

下面分情况对它进行讨论：

1、trait是一个变量，trait variable

2、trait是一个method，trait function

3、trait是一个type，trait type

参见[Example.md](./Example.md)，其中发布列举了对应上述三种情况的例子，这些例子回答了上述问题。



## C++14 TransformationTrait

C++14 TransformationTrait对trait进行强化，参见`C++\What-is-new-in-C++\C++14\TransformationTrait`。



## TO READ

http://blog.aaronballman.com/2011/11/a-simple-introduction-to-type-traits/


https://www.modernescpp.com/index.php/tag/type-traits

https://www.modernescpp.com/index.php/templates-misconceptions-and-surprises

https://www.modernescpp.com/index.php/c-core-guidelines-programming-at-compile-time-with-type-traits-ii

https://www.modernescpp.com/index.php/type-traits-performance-matters

https://www.modernescpp.com/index.php/compare-and-modify-types

https://www.modernescpp.com/index.php/check-types