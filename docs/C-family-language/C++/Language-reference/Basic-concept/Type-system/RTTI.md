# Run-time type information

对于polymorphic type（参见`C++\Language-reference\Basic-concept\Type-system\Type-system\Type-system.md#Polymorphic type`），目前的实现普遍化会为其提供RTTI。

## 维基百科[Run-time type information](https://en.wikipedia.org/wiki/Run-time_type_information)



## cppreference 

在cppreference的如下章节中，谈及了RTTI：

- [Object#Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)
- [dynamic_cast conversion#Notes](https://en.cppreference.com/w/cpp/language/dynamic_cast#Notes)
- [typeid operator](https://en.cppreference.com/w/cpp/language/typeid)



C++ features that depend on RTTI

- `typeid` 

- `dynamic_cast`
- [std::any](https://en.cppreference.com/w/cpp/utility/any) （在panicsoftware [DYNAMIC_CAST AND TYPEID AS (NON) RTTI TOOLS.](https://blog.panicsoftware.com/dynamic_cast-and-typeid-as-non-rtti-tools/) 中提及）

