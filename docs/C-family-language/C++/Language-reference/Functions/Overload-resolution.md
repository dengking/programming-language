# Overload resolution



## cppreference [overload resolution](https://en.cppreference.com/w/cpp/language/overload_resolution)





需要梳理清楚overload resolution、template function之间的关系；substitution和overload resolution的发生顺序。



overload resolution

在下面的文章中，谈及了overload resolution：
https://en.cppreference.com/w/cpp/language/sfinae
This rule applies during overload resolution of function templates: When substituting the explicitly specified or deduced type for the template parameter fails, the specialization is discarded from the overload set instead of causing a compile error.


https://www.bfilipek.com/2016/02/notes-on-c-sfinae.html#overload-resolution

When the compiler tries to compile a function call (simplified):

Perform a name lookup
For function templates the template argument values are deduced from the types of the actual arguments passed into the function.
All occurrences of the template parameter (in the return type and parameters types) are substituted with those deduced types.
When this process leads to invalid type (like int::internalType) the particular function is removed from the overload resolution set. (SFINAE)
At the end, we have a list of viable functions that can be used for the specific call. If this set is empty, then the compilation fails. If more than one function is chosen, we have an ambiguity. In general, the candidate function, whose parameters match the arguments most closely is the one that is called.

https://akrzemi1.wordpress.com/2015/11/19/overload-resolution/

http://foonathan.github.io/blog/2015/11/30/overload-resolution-4.html



## TO READ

https://akrzemi1.wordpress.com/2015/11/19/overload-resolution/

