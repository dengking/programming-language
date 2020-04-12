# Dynamic memory management

C++中的dynamic memory management主要是通过[new](https://en.cppreference.com/w/cpp/language/new)、[delete](https://en.cppreference.com/w/cpp/language/delete)，它们都是operator，是语言内置的，用户无法替换实现，C++标准库提供的[Dynamic memory management library](https://en.cppreference.com/w/cpp/memory)，可以实现更加高级的dynamic memory management。

C中的dynamic memory management是通过标准库所提供[dynamic memory management library](https://en.cppreference.com/w/c/memory)，如[malloc](https://en.cppreference.com/w/c/memory/malloc)、[free](https://en.cppreference.com/w/c/memory/free)，显然它们都是function。这样的安排，就给用户提供使用替换方案的机会，比如redis就没有使用C标准库所提供的malloc和free，而是使用的[jemalloc](http://jemalloc.net/)库，这在[redis](https://github.com/antirez/redis)中进行了介绍。