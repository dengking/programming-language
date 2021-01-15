# [Mazrog](https://github.com/Mazrog)/**[srlzio](https://github.com/Mazrog/srlzio)**

> 看了一下这个library的用法，它是非常有特点的:
>
> 1、让programmer能够显式地描述XML的structure，使用的是C++ language；在下面的reference章节描述了它的data structure和实现细节。
>
> 
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

## [Reference](https://github.com/Mazrog/srlzio#reference)

> NOTE: XML是典型的符合nesting关系的，类似于formal language CFG，因此整个XML对应的是一棵树；这个library的实现，其实也是利用了这一点，它的整体思路如下:
>
> 1、首先描述**structure/grammar**
>
> 在使用这个library的时候，因此，在使用这个library的时候，首先描述最最底层的node，然后基于它，描述**包含/contain**它的parent node；在OOP中，我们一般使用composition来描述**包含/contain**关系，即将sub node作为node的member；这个class充分使用TMP，使用template parameter来描述；
>
> 每个structure的属性，都使用template parameter来进行指定，显然这些都是static的，因为是提前知道的；
>
> 2、top down parsing
>
> 对于一个XML file，其实就是一个符合**structure/grammar**的program；这个program使用的是top down parsing；
>
> 它的source code也是基于上述两个部分来进行实现、组织的。

srlzio gives you three main structures to represent your data, `leaf`, `node`, and `collection`, the latter being a logical one (ie not an existing Xml element in the file).

### # Leaf

> NOTE: leaf是包含value的，因此需要指定它的value的type，作者将它作为了第一个template parameter

### # Node

A `node` is basicly a `leaf` but with **children** instead of a value.

### # Collection

A `collection` is a logical structure used to contain multiple identical items (ie with the same tagname).

> NOTE: collection主要用于实现contain many关系

## Parsing

Once your structures are created, you can load your file, get the `XmlElement *` corresponding to the root of your model, and call the `parse` function!

> NOTE: 这段话让我想到了: 
>
> 1、make it computational-Structuralization
>
> 2、formal language and  CFG

