# Definitions and ODR

本文着重讨论ODR。

## cppreference [Definitions and ODR](https://en.cppreference.com/w/cpp/language/definition)



## Requirement and implementation of ODR

### Requirement 

ODR要求唯一性，那它的具体requirement是怎样的？这个问题在learncpp [6.6 — Internal linkage](https://www.learncpp.com/cpp-tutorial/internal-linkage/) `#` **The one-definition rule and internal linkage** 章节进行了总结：

The one-definition rule and internal linkage

In lesson [2.7 -- Forward declarations and definitions](https://www.learncpp.com/cpp-tutorial/forward-declarations/), we noted that the **one-definition rule** says that an object or function can’t have more than one definition, either within a file or a program.

> NOTE:  上面已经告诉了我们，ODR的requirement包括：
>
> - ODR within a file
> - ODR within the whole program
>
> 

However, it’s worth noting that **internal objects** (and **functions**) that are defined in different **files** are considered to be independent entities (even if their names and types are identical), so there is no violation of the **one-definition rule**. Each **internal object** only has one definition.

### Implementation 

在前我们已经知道了ODR的requirement，显然ODR的实现依赖于：

- compiler来保证ODR
- linker来保证ODR





## TO READ

https://cpluspluspedia.com/en/tutorial/4907/one-definition-rule--odr-

https://en.cppreference.com/w/cpp/language/definition

https://en.cppreference.com/w/cpp/language/declarations