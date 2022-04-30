# Capture by this

"capture by this" 是在 stackoverflow [C++ lambda capture this vs capture by reference](https://stackoverflow.com/questions/33575563/c-lambda-capture-this-vs-capture-by-reference) # [A](https://stackoverflow.com/a/33576211/10173843) 中提出的:

> For the specific example you've provided, capturing by `this` is what you want. 



## stackoverflow [C++ lambda capture this vs capture by reference](https://stackoverflow.com/questions/33575563/c-lambda-capture-this-vs-capture-by-reference)

[A](https://stackoverflow.com/a/33576211/10173843)

For the specific example you've provided, capturing by `this` is what you want. 

> NOTE: 
>
> "capturing by `this` " 是一种新的说法。

Conceptually, capturing `this` by reference doesn't make a whole lot of sense, since you can't change the value of `this`, you can only use it as a pointer to access members of the class or to get the address of the class instance.

> NOTE: 
>
> 作者的观点非常明确："capturing `this` by reference doesn't make a whole lot of sense"

Inside your lambda function, if you access things which implicitly use the `this` pointer (e.g. you call a member function or access a member variable without explicitly using `this`), the compiler treats it as though you had used `this` anyway. You can list multiple captures too, so if you want to capture both members and local variables, you can choose independently whether to capture them by reference or by value. The following article should give you a good grounding in lambdas and captures:

https://crascit.com/2015/03/01/lambdas-for-lunch/

Also, your example uses `std::function` as the return type through which the lambda is passed back to the caller. Be aware that `std::function` isn't always as cheap as you may think, so if you are able to use a lambda directly rather than having to wrap it in a `std::function`, it will likely be more efficient. The following article, while not directly related to your original question, may still give you some useful material relating to lambdas and `std::function` (see the section *An alternative way to store the function object*, but the article in general may be of interest):

https://crascit.com/2015/06/03/on-leaving-scope-part-2/



[A](https://stackoverflow.com/a/63167399/10173843)

It's not a clear-cut situation where on is better than the other. Rather, the two (at least potentially) accomplish slightly different things. For example, consider code like this:

```c++
#include <iostream>
#include <string>
#include <unordered_map>
#include <array>
#include <vector>
#include <string>
#include <iostream>
#include <variant>
#include <typeinfo>
#include <limits>
using namespace std;
#include <iostream>

class foo
{
    int bar = 0;

public:
    void baz()
    {
        int bar = 1;
        auto thing1 = [&]
        { bar = 2; };
        auto thing2 = [this]
        { this->bar = 3; };

        std::cout << "Before thing1: local bar: " << bar << ", this->bar: " << this->bar << "\n";
        thing1();
        std::cout << "After thing1: local bar: " << bar << ", this->bar: " << this->bar << "\n";
        thing2();
        std::cout << "After thing2: local bar: " << bar << ", this->bar: " << this->bar << "\n";
    }
};

int main()
{
    foo f;
    f.baz();
}
```



```c++
Before thing1: local bar: 1, this->bar: 0
After thing1: local bar: 2, this->bar: 0
After thing2: local bar: 2, this->bar: 3
```

