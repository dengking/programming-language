# `assign` and `operator=`

1、是在思考virtual `operator=`时，在 CppCoreGuidelines [C.60: Make copy assignment non-`virtual`, take the parameter by `const&`, and return by non-`const&`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c60-make-copy-assignment-non-virtual-take-the-parameter-by-const-and-return-by-non-const)

中，有这样的描述:

> **Alternatives**: If you think you need a `virtual` assignment operator, and understand why that's deeply problematic, don't call it `operator=`. Make it a named function like `virtual void assign(const Foo&)`. See [copy constructor vs. `clone()`](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-copy-virtual)

这引发了我对`assign` and `operator=`的思考





## stackoverflow [std::string::assign vs std::string::operator=](https://stackoverflow.com/questions/34196053/stdstringassign-vs-stdstringoperator)

