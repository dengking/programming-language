# VS inline VS macro

[Inline functions vs Preprocessor macros](https://stackoverflow.com/questions/1137575/inline-functions-vs-preprocessor-macros)





[In C++, what’s the difference between an inline function and a macro?](https://www.programmerinterview.com/c-cplusplus/inline-vs-macro/)





## marco的优势

在阅读[offsetof](https://en.wikipedia.org/wiki/Offsetof)这篇文章的时候，其中提及了这样一段话：

> C's **offsetof()** macro is an [ANSI C](https://en.wikipedia.org/wiki/ANSI_C) library feature found in `stddef.h`. It evaluates to the offset (in bytes) of a given member within a [struct](https://en.wikipedia.org/wiki/Struct) or [union](https://en.wikipedia.org/wiki/Union_(computer_science)) type, an expression of type `size_t`. The `offsetof`() macro takes two [parameters](https://en.wikipedia.org/wiki/Parameters), the first being a structure name, and the second being the name of a member within the structure. It cannot be described as a C prototype.[[1\]](https://en.wikipedia.org/wiki/Offsetof#cite_note-1)

即如果使用函数的话，`offsetof` cannot be described as a C prototype，所以才使用marco；

