# ODR

compiler需要从多个层次来保证ODR：

1) 单个translation unit内，是由compiler来保证的

2) 整个target file内，是由linker来保证的

在这种情况下，linker的作用对象是**symbol with external linkage**，对于**symbol with internal linkage**，由于它们是**translation local**的，linker是不会对它们进行检查的。

准确的把我了ODR的含义，就能够理解multiple definition的情况了: 在C++、C中，是允许同时存在多个translation中存在着translation local的definition的。

关于linkage，参见`C-and-C++\From-source-code-to-exec\Link\Linkage`。

## cppreference [Definitions and ODR (One Definition Rule)#One Definition Rule](https://en.cppreference.com/w/cpp/language/definition)

Only one definition of any variable, function, class type, enumeration type, [concept](https://en.cppreference.com/w/cpp/language/constraints) (since C++20) or template is allowed in any one translation unit (some of these may have multiple declarations, but only one definition is allowed).

> NOTE: 这就是前面描述的1) 单个translation unit内。

One and only one definition of every non-[inline](https://en.cppreference.com/w/cpp/language/inline) function or variable that is *odr-used* (see below) is required to appear in the entire program (including any standard and user-defined libraries). The compiler is not required to diagnose this violation, but the behavior of the program that violates it is undefined.

> NOTE: 这就是前面描述的整个target file内。

For an **inline function** or **inline variable** (since C++17), a definition is required in every **translation unit** where it is *odr-used*.

> NOTE: 对于inline function、inline variable，在每个definition中都需要一个definition，C++语言规定这是没有违反ODR的；在这种情况下，compiler是有多种实现方式的:
>
> 1) 将definition设置为translation local/internal linkage
>
> 2) 将函数inline

One and only one definition of a **class** is required to appear in any translation unit where the class is used in a way that requires it to be [complete](https://en.cppreference.com/w/cpp/language/incomplete_type).



There can be more than one definition in a program of each of the following: class type, enumeration type, inline function, inline variable (since C++17), [templated entity](https://en.cppreference.com/w/cpp/language/templates#Templated_entity) (template or member of template, but not full [template specialization](https://en.cppreference.com/w/cpp/language/template_specialization)), as long as all of the following is true:

> NOTE: 需要注意的是，上面这段话所强调的是"in a program "



> NOTE: 原文这里给出了一些列描述的规则



If all these requirements are satisfied, the program behaves as if there is only one definition in the entire program. Otherwise, the program is ill-formed, no diagnostic required.

> NOTE: 这是C++语言的特殊规定



## wikipedia [One Definition Rule](https://en.wikipedia.org/wiki/One_Definition_Rule)

