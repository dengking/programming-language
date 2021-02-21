# Generic container printer

如何实现通用的输出container的函数？显然是一些实现方式:

1、Overload output operator + container as template parameter + variadic templates 

参见: stackoverflow [What are some uses of template template parameters?](https://stackoverflow.com/questions/213761/what-are-some-uses-of-template-template-parameters) # [A](https://stackoverflow.com/a/14311714)

收录于: `Template-template-parameter` 章节。

2、`std::copy` + `std::ostream_iterator` 

参见: 

a、thispointer [How to copy all Values from a Map to a Vector in C++](https://thispointer.com/how-to-copy-all-values-from-a-map-to-a-vector-in-c/)

b、stackoverflow [Getting a list of values from a map](https://stackoverflow.com/questions/4195611/getting-a-list-of-values-from-a-map) # [A](https://stackoverflow.com/a/4195743)

收录于: `Values-like-python-dict` 章节。

3、`std::transform` + `std::ostream_iterator`

参见:

a、stackoverflow [Copy map values to vector in STL](https://stackoverflow.com/questions/771453/copy-map-values-to-vector-in-stl) # [A](https://stackoverflow.com/a/771482)

收录于: `Values-like-python-dict` 章节。