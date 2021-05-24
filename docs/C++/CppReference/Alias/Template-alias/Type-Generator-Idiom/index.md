# Type Generator



## [More C++ Idioms/Type Generator](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Generator)



### Solution and Sample Code

```c++
template <class Value>
struct Directory
{
  typedef std::map <std::string, Value, std::less<std::string>, 
                    __gnu_cxx::malloc_allocator<std::string> > type;
};

Directory<int>::type    // gives a map of string to integers.
Directory<float>::type  // gives a map of string to floats.
```



```c++
Directory<int>::type age; // This is a map.
transform(age.begin(), age.end(),
          std::ostream_iterator<string>(std::cout, "\n"),
          _Select1st<std::map<std::string, int>::value_type> ());
```



```c++
template <class Value>
struct Directory
{
  typedef map <string, Value, less<string>, __gnu_cxx::malloc_allocator<std::string> > type;
  typedef _Select1st<typename type::value_type> KeySelector;
  typedef _Select2nd<typename type::value_type> ValueSelector;
};
Directory<int>::type age;    // This is a map.
transform(age.begin(), age.end(),
          std::ostream_iterator<string>(std::cout, "\n"),
          Directory<int>::KeySelector());
```



## Template aliases

C++11的[Template aliases](https://en.wikipedia.org/wiki/C++11#Template_aliases)能够用于实现type generator。

