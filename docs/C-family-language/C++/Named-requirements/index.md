# Named requirements

在`Theory\Programming-language\Design-of-programming-language.md`的《[Design by contact](https://en.wikipedia.org/wiki/Design_by_contract) and expectation and generic programming》章节中，已经提及了c++的named requirement，按照其中的思想来理解cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)是非常容易的。

需要注意的是，c++ named requirement是generic programming的concept，在c++20中它们都可以使用concept feature进行formalize。（基本上）named requirement是通过对应的magic function来实现的，也就是它是基于behavior，而非基于type的，在文章`Comparison-of-programming-language\Templates-and-Duck-Typing\Cpp-VS-python-generic-programming.md`中对此进行了详细分析。

## cppreference [Named requirements](https://en.cppreference.com/w/cpp/named_req)