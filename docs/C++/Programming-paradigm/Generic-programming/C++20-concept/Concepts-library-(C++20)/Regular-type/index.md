# Regular type



## modernescpp [C++ Core Guidelines: Regular and SemiRegular Types](https://www.modernescpp.com/index.php/c-core-guidelines-regular-and-semiregular-typs)





## stackoverflow [What is a “Regular Type” in the context of move semantics?](https://stackoverflow.com/questions/13998945/what-is-a-regular-type-in-the-context-of-move-semantics)



## Implementation 

一、sodocumentation [C++Type Erasure](https://sodocumentation.net/cplusplus/topic/2872/type-erasure)

二、在 stlab [Small Object Optimization for Polymorphic Types](https://stlab.cc/tip/2017/12/23/small-object-optimizations.html) 中提及了 [`adobe::any_regular_t`](https://github.com/stlab/adobe_source_libraries/blob/master/adobe/any_regular.hpp) 

上述两者的implementation都使用了:

1、custom virtual table

2、type erasure