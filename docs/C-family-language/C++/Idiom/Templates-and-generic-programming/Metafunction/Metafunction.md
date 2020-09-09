# Metafunction

“metafunction”即“元函数”，它用于template meta-programming。

本质上来说，它并不是function，但是从功能上来看，它类似于function: 

用途一: metafunction是一个抽象的interface，或者说，它是一种抽象，通过它，可以获取模板参数的信息，因为我们可以认为它是在compile-time“执行”的函数，这也是它称为metafunction的原因。最最典型的metafunction就是：trait。

> NOTE: metafunction还是可以实现其他功能，后续补充。



## Example

### `trait`

前面已经提及了trait，关于trait，参见`C++\Idiom\Templates-and-generic-programming\SFINAE-trait-enable-if`。

### `std::enable_if`

在cppreference `std::enable_if`中的介绍如下:

> This metafunction is a convenient way to leverage SFINAE to conditionally remove functions from overload resolution based on type traits and to provide separate function overloads and specializations for different type traits.

### `std::void_t`

在cppreference `std::void_t`中的介绍如下:

> Utility metafunction that maps a sequence of any types to the type void

### cppreference `Utility library#Type support#Metafunctions`

在cppreference 中介绍了metafunction。

## Implementation

metafunction是基于C++ template的，所以它的实现是基于C++ template的实现的，参见`C++\Language-reference\Template\Implementation`。



## [More C++ Idioms](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Metafunction) [Metafunction](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Metafunction)



## boost [Metafunction](https://www.boost.org/doc/libs/1_41_0/libs/mpl/doc/refmanual/metafunctions-concepts.html#id1495)



## akrzemi1 [Meta-functions in C++11](https://akrzemi1.wordpress.com/2012/03/19/meta-functions-in-c11/)





## TO READ

[C++: are “trait” and “meta-function” synonymous?](https://stackoverflow.com/questions/32471222/c-are-trait-and-meta-function-synonymous)



[An In-Depth Look at Metafunctions in C++](https://www.informit.com/articles/article.aspx?p=375705)