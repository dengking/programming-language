# Best practices for exception handling



`std::nothrow`是一个constant；`noexcpet`是一个key word；

## vishalchovatiya [C++ Exception Handling Best Practices: 7 Things To Know](http://www.vishalchovatiya.com/7-best-practices-for-exception-handling-in-cpp-with-example/)







## Destructor non-throwing

今天在使用pclint来检查待目的时候，有如下告警:

```c++
D:\hs\UST\OptUst\trunk\Sources\sys\opt\api\HSTradeApi\cpp_sqlite3\CppSQLite3.cpp  
    142  warning 1551: function 'CppSQLite3Buffer::clear' called outside of a 
    try block in destructor 'CppSQLite3Buffer::~CppSQLite3Buffer' is not 
    declared as never throwing
        clear();
```

在C++ Core Guidelines中，有关于destructor、exception的讨论

## why std::swap non-throwing



http://cpptruths.blogspot.de/2011/09/tale-of-noexcept-swap-for-user-defined.html



## move and swap and no-throw

http://www.gotw.ca/gotw/059.htm

http://cpptruths.blogspot.com/2011/09/tale-of-noexcept-swap-for-user-defined.html

https://zhehao.me/effectives/ecpp/it25/

https://blog.ycshao.com/2017/10/22/effective-c-item-25-consider-support-for-a-non-throwing-swap/

https://rules.sonarsource.com/cpp/tag/cpp11/RSPEC-5018



https://github.com/isocpp/CppCoreGuidelines/blob/036324/CppCoreGuidelines.md#c66-make-move-operations-noexcept

https://github.com/isocpp/CppCoreGuidelines/blob/036324/CppCoreGuidelines.md#c85-make-swap-noexcept

https://github.com/isocpp/CppCoreGuidelines/blob/036324/CppCoreGuidelines.md#Rc-swap-fail



https://stackoverflow.com/questions/36010810/implementing-the-swap-in-the-copy-and-swap-idiom



https://stackoverflow.com/questions/27665995/on-noexcept-arguments