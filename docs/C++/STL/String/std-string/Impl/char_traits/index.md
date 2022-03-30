# char_traits

是在阅读[C++ Type Traits | Dr Dobb's](http://www.drdobbs.com/cpp/c-type-traits/184404270)时，发现其中有对`std::char_trait`的描述。

1、`char_traits`是典型的policy-based design。

2、`char_traits`所封装的是algorithm

## cppreference [std::char_traits](https://en.cppreference.com/w/cpp/string/char_traits)

The `char_traits` class is a traits class template that abstracts basic character and string operations for a given character type.

> NOTE: 在c++ standard library中，基本上，所有涉及basic character and string operation的，都是由[std::char_traits](https://en.cppreference.com/w/cpp/string/char_traits)提供的。

The defined operation set is such that generic algorithms almost always can be implemented in terms of it. It is thus possible to use such algorithms with almost any possible character or string type, just by supplying a customized `char_traits` class.

> NOTE: 这段话的中心思想是：`char_traits`的设计思想是支持generic algorithms



## stackoverflow [What is the point of STL Character Traits?](https://stackoverflow.com/questions/5319770/what-is-the-point-of-stl-character-traits)

> NOTE: 
>
> 1、这篇文章非常好

### [A](https://stackoverflow.com/a/5319855)

Character traits are an extremely important component of the streams and strings libraries because they allow the stream/string classes to separate out the logic of *what characters are being stored* from the logic of *what manipulations should be performed on those characters.*

> NOTE: 
>
> 1、separation of structure and algorithm，下面的`CaseInsensitiveString`就是这样的典型例子

To begin with, the default character traits class, `char_traits<T>`, is used extensively in the C++ standard. For example, there is no class called `std::string`. Rather, there's a class template `std::basic_string` that looks like this:

```c++
template <typename charT, typename traits = char_traits<charT> >
    class basic_string;
```

Then, `std::string` is defined as

```c++
typedef basic_string<char> string;
```

Similarly, the standard streams are defined as

```c++
template <typename charT, typename traits = char_traits<charT> >
    class basic_istream;

typedef basic_istream<char> istream;
```

So why are these classes structured as they are? Why should we be using a weird traits class as a template argument?

The reason is that in some cases we might want to have a string just like `std::string`, but with some slightly different properties. One classic example of this is if you want to store strings in a way that ignores case. For example, I might want to make a string called `CaseInsensitiveString` such that I can have

```c++
CaseInsensitiveString c1 = "HI!", c2 = "hi!";
if (c1 == c2) {  // Always true
    cout << "Strings are equal." << endl;
}
```

That is, I can have a string where two strings differing only in their case sensitivity are compared equal.

Now, suppose that the standard library authors designed strings without using traits. This would mean that I'd have in the standard library an immensely powerful string class that was entirely useless in my situation. I couldn't reuse much of the code for this string class, since comparisons would always work against how I wanted them to work. But by using traits, it's actually possible to reuse the code that drives `std::string` to get a case-insensitive string.

> NOTE: 
>
> 1、翻译如下:
>
> "现在，假设标准库作者在设计字符串时没有使用特征。
> 这意味着在标准库中有一个非常强大的字符串类，在我的情况下完全没用。
> 我不能重用这个string类的大部分代码，因为比较总是与我希望它们的工作方式相反。
> 但是通过使用特征，实际上可以重用驱动' std::string '的代码来获得不区分大小写的字符串。"

If you pull up a copy of the C++ ISO standard and look at the definition of how the string's comparison operators work, you'll see that they're all defined in terms of the `compare` function. This function is in turn defined by calling

```
traits::compare(this->data(), str.data(), rlen)
```

where `str` is the string you're comparing to and `rlen` is the smaller of the two string lengths. This is actually quite interesting, because it means that the definition of `compare` directly uses the `compare` function exported by the traits type specified as a template parameter! Consequently, if we define a new traits class, then define `compare` so that it compares characters case-insensitively, we can build a string class that behaves just like `std::string`, but treats things case-insensitively!

Here's an example. We inherit from `std::char_traits<char>` to get the default behavior for all the functions we don't write:

```C++
class CaseInsensitiveTraits: public std::char_traits<char> {
public:
    static bool lt (char one, char two) {
        return std::tolower(one) < std::tolower(two);
    }

    static bool eq (char one, char two) {
        return std::tolower(one) == std::tolower(two);
    }

    static int compare (const char* one, const char* two, size_t length) {
        for (size_t i = 0; i < length; ++i) {
            if (lt(one[i], two[i])) return -1;
            if (lt(two[i], one[i])) return +1;
        }
        return 0;
    }
};
```

(Notice I've also defined `eq` and `lt` here, which compare characters for equality and less-than, respectively, and then defined `compare` in terms of this function).

Now that we have this traits class, we can define `CaseInsensitiveString` trivially as

```c++
typedef std::basic_string<char, CaseInsensitiveTraits> CaseInsensitiveString;
```

And voila! We now have a string that treats everything case-insensitively!

Of course, there are other reasons besides this for using traits. For example, if you want to define a string that uses some underlying character type of a fixed-size, then you can specialize `char_traits` on that type and then make strings from that type. In the Windows API, for example, there's a type `TCHAR` that is either a narrow or wide character depending on what macros you set during preprocessing. You can then make strings out of `TCHAR`s by writing

```
typedef basic_string<TCHAR> tstring;
```

And now you have a string of `TCHAR`s.

In all of these examples, notice that we just defined some traits class (or used one that already existed) as a parameter to some template type in order to get a string for that type. The whole point of this is that the `basic_string` author just needs to specify how to use the traits and we magically can make them use our traits rather than the default to get strings that have some nuance or quirk not part of the default string type.

Hope this helps!

**EDIT**: As @phooji pointed out, this notion of traits is not just used by the STL, nor is it specific to C++. As a completely shameless self-promotion, a while back I wrote [an implementation of a ternary search tree](http://www.keithschwarz.com/interesting/code/?dir=ternary-search-tree) (a type of radix tree [described here](http://en.wikipedia.org/wiki/Ternary_search_tree)) that uses traits to store strings of any type and using whatever comparison type the client wants them to store. It might be an interesting read if you want to see an example of where this is used in practice.

**EDIT**: In response to your claim that `std::string` doesn't use `traits::length`, it turns out that it does in a few places. Most notably, when you construct a `std::string` out of a `char*` C-style string, the new length of the string is derived by calling `traits::length` on that string. It seems that `traits::length` is used mostly to deal with C-style sequences of characters, which are the "least common denominator" of strings in C++, while `std::string` is used to work with strings of arbitrary contents.

## use of `char_traits` in standard library

[std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string)

[Input/output library](Input/output library)