# ODR

## ODR的含义和implementation

- ODR within a file
- ODR within the whole program

compiler需要从多个层次来保证ODR：

1) ODR within translation unit，是由compiler来保证的

2) ODR within the whole target file，是由linker来保证的

在这种情况下，linker的作用对象是**symbol with external linkage**，对于**symbol with internal linkage**，由于它们是**translation local**的，linker是不会对它们进行检查的。

关于linkage，参见`C-and-C++\From-source-code-to-exec\Link\Linkage`。

### ODR and multiple definition

ODR中的"O"的含义是one，而在C++中，有的时候允许出现multiple definition，显然这看似是矛盾的。上面结合implementation对此进行分析，显然: 

在C++、C中，是允许同时存在多个translation中存在着**translation local**的definition的。

这个问题在learncpp [6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/) `#` **The one-definition rule and internal linkage** 章节进行了总结：

> The one-definition rule and internal linkage
>
> In lesson [2.7 -- Forward declarations and definitions](https://www.learncpp.com/cpp-tutorial/forward-declarations/), we noted that the **one-definition rule** says that an object or function can’t have more than one definition, either within a file or a program.
>
> However, it’s worth noting that **internal objects** (and **functions**) that are defined in different **files** are considered to be independent entities (even if their names and types are identical), so there is no violation of the **one-definition rule**. Each **internal object** only has one definition.



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

> NOTE: 这是C++语言的特殊规定，这种情况下实际上是有多个definition，但是C++语言规定将它们视为同一个definition。



## wikipedia [One Definition Rule](https://en.wikipedia.org/wiki/One_Definition_Rule)



## TODO

https://cpluspluspedia.com/en/tutorial/4907/one-definition-rule--odr-





