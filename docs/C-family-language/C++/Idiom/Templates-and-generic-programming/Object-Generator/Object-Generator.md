# Object Generator



## More C++ Idioms/[Object Generator](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Object_Generator)

### Intent

To simplify creation of objects without explicitly specifying their types. (This is not the factory method pattern)

> NOTE: factory method是OOP的design pattern



### Solution and Sample Code

In the *object generator* idiom, a template function is created whose only job is to construct a new object from its parameters. It is based on a useful property of function templates which class templates don't have: The type parameters of a function template are deduced automatically from its actual parameters. 

> NOTE: 充分运用function template的automatically deduce parameter特性，进而实现简化代码的目的

```c++
template <class T, class U>
std::pair <T, U> 
make_pair(T t, U u)
{
  return std::pair <T, U> (t,u);
}
```

The C++ standard library defines several object generators to avoid code bloat. *std::bind2nd* and [*std::mem_fun*](https://en.cppreference.com/w/cpp/utility/functional/mem_fun) are two such standard object generators that can be used to avoid code bloat in the example shown in the motivation section above.

```c++
void read_lines(std::vector<File *> files)
{
   std::string arg;
   for_each(files.begin(), files.end(), bind2nd(mem_fun(&File::read_line), arg));
}
```

### Known Uses

[std::make_pair](https://en.cppreference.com/w/cpp/utility/pair/make_pair)

[std::make_shared](https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared)



### References

[Object Generator](http://www.boost.org/community/generic_programming.html#object_generator)

> NOTE: 这个链接到的是boost [Generic Programming Techniques#TYPE GENERATOR](https://www.boost.org/community/generic_programming.html#type_generator)，我是在阅读这篇文章的时候，发现了这个idiom的。

