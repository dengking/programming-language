# [Mazrog](https://github.com/Mazrog)/**[srlzio](https://github.com/Mazrog/srlzio)**

> 看了一下这个library的用法，它是非常有特点的:
>
> 1、让programmer能够显示的描述XML的structure，使用的是C++ language
>
> 2、它的实现，极大的使用了C++ template，可以作为学习的典范
>
> 3、它的实现是需要C++17的，因为它使用了如下特性:
>
> a、
>
> ```C++
> template < std::size_t I >
> auto & get() { return std::get<I>(values); }
> ```
>
> b、
>
> 
>
> ```C++
> std::index_sequence
> std::make_index_sequence
> ```
>
> 
>
> c、
>
> ```C++
> if constexpr (sizeof...(Attrs))
> ```
>
> 





## Get Started

As it is a header-only library, download the latest version of the file and include it in your project and as it uses the tinyxml2 library, be sure you have it installed and linked to your project.

Let's consider this example: we have a file that contains multiple IDs informations (name and age) about employees for an certain office (name). In XML, it could be written this way

```xml
<?xml version="1.0" encoding="utf-8" ?>
<OfficeInfo>
  <Office>
    <Name>Splendid Office</Name>
  </Office>
  <Employees>
    <Employee id="0" gender="H">
      <Name>John</Name>
      <Age>42</Age>
    </Employee>
    <Employee id="1" gender="F">
      <Name>Jane</Name>
      <Age>36</Age>
    </Employee>
    <Employee id="2" gender="F">
      <Name>Meryl</Name>
      <Age>40</Age>
    </Employee>
  </Employees>
</OfficeInfo>
```

We create the model-structure in C++, from the leaves to the root:

```C++
// models.hpp
#include "srlzio.hpp"

struct Employee : xml::node<
    xml::attributes<int, std::string>,
    xml::leaf<std::string, xml::attributes<>>,
    xml::leaf<int, xml::attributes<>>
  > {
    Employee() {
      attributes.names = { "id", "gender" };
      children.names = { "Name", "Age" };
    }
};
```

> NOTE: `struct Employee` 对应的是上述XML中的`Employee`

```C++
struct Employees : xml::node<xml::attributes<>, xml::collection<Employee>> {
  Employees() { children.names = { "Employee" }; }
};
```

> NOTE: `struct Employees` 对应的是上述XML中的`Employees`

```C++
struct Office : xml::node<xml::attributes<>, xml::leaf<std::string, xml::attributes<>>> {
  Office() { children.names = { "Name" }; }
};
```

> NOTE: `struct Office`对应上述XML中的`Office`



```C++
struct OfficeInfo : xml::node<xml::attributes<>, Office, Employees> {
  OfficeInfo() { children.names = { "Office", "Employees" }; }
};
```

