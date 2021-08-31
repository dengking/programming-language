# struct RTTI

我的需求: 当面对非常多的`struct`的时候，如果能够使用tool来自动地解析这些`struct`，然后生成对应的类型信息，这将极大地提升效率。



## Parse c++ header file

Google:

1、parse c++ header file

2、C/C++ header file parser

下面是检索的有价值的结果: 

### senexcanis [CppHeaderParser](http://senexcanis.com/open-source/cppheaderparser/)

1、这是Google parse c++ header file的第一个返回结果。

2、https://pypi.org/project/CppHeaderParser/

3、我最终选择了这个library

4、借助它，实现了一个auxiliary compiler，生成struct RTTI

### thegreenplace [Parsing C++ in Python with Clang](https://eli.thegreenplace.net/2011/07/03/parsing-c-in-python-with-clang)

### stackoverflow [Is there a good Python library that can parse C++? [closed]](https://stackoverflow.com/questions/1444961/is-there-a-good-python-library-that-can-parse-c)



### github [gccxml](http://gccxml.github.io/HTML/Index.html)



## Library

library [visit_struct](https://github.com/garbageslam/visit_struct)

library [magic_get](https://github.com/apolukhin/magic_get)

github baszalmstra [header-parser](https://github.com/baszalmstra/header-parser)

github Leandros [metareflect](https://github.com/Leandros/metareflect)

foonathan [cppast](https://github.com/foonathan/cppast)



## Posts

meetingcpp [Building a C++ Reflection System](https://meetingcpp.com/mcpp/slides/2018/Reflection2.pdf)

duriansoftware [Self-aware struct-like types in C++11](http://duriansoftware.com/joe/Self-aware-struct-like-types-in-C++11.html)

stackoverflow [How can I add reflection to a C++ application?](https://stackoverflow.com/questions/41453/how-can-i-add-reflection-to-a-c-application)

eliben [llvm-clang-samples](https://github.com/eliben/llvm-clang-samples)

cwoodall [Reflection in C++ to Generate Serializable Structs Using libclang and Python](http://cwoodall.com/blog/2018/02/24/using-clang-and-python-to-generate-cpp-struct-serde-fns.html)

stackoverflow [C++ JSON Serialization](https://stackoverflow.com/questions/17549906/c-json-serialization)



https://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented

