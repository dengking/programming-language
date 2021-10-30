# Parsing



## 自顶向下parsing

1、`void parse(tinyxml2::XMLElement * elem, xml::collection<Item> & collection)`

2、`void parse(tinyxml2::XMLElement * elem, xml::node<xml::attributes<Attrs...>, Children...> & node)`

3、`void parse(tinyxml2::XMLElement * elem, xml::leaf<T, xml::attributes<Attrs...>> & leaf)`



## attributes

attributes对应是`struct dict`，attributes的值使用的是`std::tuple`，解析attributes是非常简单的: 

同时遍历`struct dict`的`names`、`values`，它对应的就是一个for循环，只不过，它是在compile time实现的，显然这是compile-time computation，下面是实现代码，它使用了`std::index_sequence`技巧来实现的: 

```C++
template < typename ... Attrs, std::size_t ... I >
void read_attrs(tinyxml2::XMLElement * elem, xml::dict<Attrs...> & attributes, std::index_sequence<I...>) {
    (load(elem, attributes.names[I], std::get<I>(attributes.values)), ...);
}
```



### `std::tuple` VS `std::vector`

`std::tuple`的元素类型、长度在compile time就已经知道了；

`std::vector`的元素类型在compile time已经知道了，但是它的长度在compile time未知；



## trait/consistent interface

```C++
template < typename T >
void read(tinyxml2::XMLElement *, T *);

template < >
void read<double>(tinyxml2::XMLElement * elem, double * d) { elem->QueryDoubleText(d); }

template < >
void read<int>(tinyxml2::XMLElement * elem, int * i) { elem->QueryIntText(i); }

template < >
void read<std::string>(tinyxml2::XMLElement * elem, std::string * str) { *str = elem->GetText(); }

/* --------------------------------------------------------------------------------------- */
template < typename T >
void load(tinyxml2::XMLElement * , std::string_view , T & );

template <>
void load<int>(tinyxml2::XMLElement * elem, std::string_view attribute_name, int & attribute) {
    elem->QueryAttribute(attribute_name.data(), &attribute);
}

template <>
void load<std::string>(tinyxml2::XMLElement * elem, std::string_view attribute_name, std::string & attribute) {
    attribute = elem->Attribute(attribute_name.data());
}
```

