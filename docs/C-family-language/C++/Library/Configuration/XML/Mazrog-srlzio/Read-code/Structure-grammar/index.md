# [srlzio](https://github.com/Mazrog/srlzio)/[srlzio](https://github.com/Mazrog/srlzio/tree/master/srlzio)/[**srlzio.hpp**](https://github.com/Mazrog/srlzio/blob/master/srlzio/srlzio.hpp)

## `struct dict`

```C++
#include <array>
#include <tuple>
#include <string>
template<typename ... Ts>
struct dict
{
	std::tuple<Ts...> values;
	std::array<std::string, sizeof...(Ts)> names;

	template<std::size_t I>
	auto& get()
	{
		return std::get<I>(values);
	}

	template<std::size_t I>
	auto const& get() const
	{
		return std::get<I>(values);
	}
};

```

显然，`dict`包含name、value；

`dict`是一个非常强大的container，通过后面可以看到:

1、`dict`是最最基础的container，(基本上)所有的高级structure都是基于它来实现的，

2、`dict`是允许recursive contain

由于`dict`抽象了element type，因此，它的element type可以是它自己，这样就可以实现recursive contain；这对于XML是非常重要的，因为XML中最最重要但就是contain关系，下面是一些简单的例子: 

a、attribute、leaf、node: node `contain` leaf `contain` attribute；

b、node就是用了`dict`的recursive contain，从code可以看出



### compile `struct dict` in C++11

上述代码，在`gcc --std=c++11 test.cpp`中是可以编译通过的，warning如下:

```C++
test.cpp:11:12: 警告：‘get’ function uses ‘auto’ type specifier without trailing return type [默认启用]
  auto& get()
            ^
test.cpp:17:20: 警告：‘get’ function uses ‘auto’ type specifier without trailing return type [默认启用]
  auto const& get() const
```

思考: 如何消除上述warning？

### `attributes`

```C++
template<typename ... Attrs>
using attributes = dict<Attrs...>;
```

可以看到，`attributes`是直接基于`dict`实现的；

## `struct tag`

它其实对应的就是XML tag，显然它可以是:

1、leaf

2、node

它们分别有自己的`struct tag` partial template specialization:

### leaf

```C++
template<bool is_node, typename ... > struct tag;

template<typename T, typename ... Attrs>
struct tag<false, T, dict<Attrs...>>
{
	T value;
	dict<Attrs...> attributes;
};

template<typename T, typename ... Attrs>
using leaf = tag<false, T, Attrs...>;

```

#### Standard format/用法如下

```C++
// Standard format:
// xml::leaf<DataType, xml::attributes<AttributesValueTypes...>>

xml::leaf<double, xml::attributes<>> pi;

xml::leaf<std::string, xml::attributes<std::string, int>> file;
```

可以看到，上述standard format和前面的partial template specialization是一致的；

#### 实现总结

显然，上述写法就实现了:

1、`struct tag<false, T, dict<Attrs...>>`的写法保证了leaf只能够包含attribute，且只能怪包含一个attribute

2、模板参数 `typename ... Attrs`其实指定的是`dict<Attrs...> attributes;`中的

3、上述这种写法，让我对C++ partial template specialization的写法有了新的认知:

a、compiler在编译的时候，应该是根据`struct tag<false, T, dict<Attrs...>>`来判断是否是partial template specialization，这种写法就表示它是partial template specialization

b、compiler是根据partial template specialization `struct tag<false, T, dict<Attrs...>>`中的`<false, T, dict<Attrs...>>`来判定template parameter，即 `template<typename T, typename ... Attrs>` 的含义的；

### node

```C++
template<bool is_node, typename ... > struct tag;

template<typename ... Attrs, typename ... Children>
struct tag<true, dict<Attrs...>, Children...>
{
	dict<Attrs...> attributes;
	dict<Children...> children;
};

template<typename ... Ts>
using node = tag<true, Ts...>;

```

#### Standard format/用法如下

```C++
// Standard format:
// xml::node<xml::attributes<AttributesValueTypes...>, Children...>

xml::node<xml::attributes<>, xml::leaf<double, xml::attributes<>>> node1;

xml::node<
  xml::attributes<int>, // parent node attributes
  xml::leaf<double, xml::attributes<std::string>>,      // first child
  xml::leaf<double, xml::attributes<std::string, int>>  // second child
  > node3;
```



可以看到，上述standard format和前面的partial template specialization是一致的；

#### 实现总结

显然，上述写法可以实现:

1、node第一个包含的必须是attribute，且只能怪包含一个

2、node可以包含多个child，这些child被放到它的`children`中

3、node的child也可以是node，这就实现了recursive contain

## `struct collection`

```C++
template<typename Item>
struct collection
{
	std::vector<Item> items;
};

```



## 完整代码

```C++
#include <array>
#include <tuple>
#include <string>
#include <vector>

namespace xml
{
template<typename ... Ts>
struct dict
{
	std::tuple<Ts...> values;
	std::array<std::string, sizeof...(Ts)> names;

	template<std::size_t I>
	auto& get()
	{
		return std::get<I>(values);
	}

	template<std::size_t I>
	auto const& get() const
	{
		return std::get<I>(values);
	}
};

template<bool is_node, typename ... > struct tag;

template<typename ... Attrs, typename ... Children>
struct tag<true, dict<Attrs...>, Children...>
{
	dict<Attrs...> attributes;
	dict<Children...> children;
};

template<typename T, typename ... Attrs>
struct tag<false, T, dict<Attrs...>>
{
	T value;
	dict<Attrs...> attributes;
};

template<typename Item>
struct collection
{
	std::vector<Item> items;
};

template<typename ... Attrs>
using attributes = dict<Attrs...>;

template<typename T, typename ... Attrs>
using leaf = tag<false, T, Attrs...>;

template<typename ... Ts>
using node = tag<true, Ts...>;

}

```

就是上述49行代码(最核心的) ，能够让我们实现描述可能非常复杂的XML的结构，这就是C++ template的威力；

## 完整例子

### data

 [srlzio](https://github.com/Mazrog/srlzio)/[data](https://github.com/Mazrog/srlzio/tree/master/data)/[**recipe.xml**](https://github.com/Mazrog/srlzio/blob/master/data/recipe.xml)



```XML
<?xml version="1.0" encoding="UTF-8" ?>
<DataFile>
    <Models>
        <ModelBuffer id="0">
            <Model id="0">
                <File>/kappa/foo</File>
            </Model>
            <Model id="1">
                <File>/kappa/bar</File>
            </Model>
        </ModelBuffer>
        <ModelBuffer id="1">
            <Model id="0">
                <File>/plop/foo</File>
            </Model>
            <Model id="1">
                <File>/plop/bar</File>
            </Model>
        </ModelBuffer>
    </Models>
</DataFile>
```



### structure

[srlzio](https://github.com/Mazrog/srlzio)/[sample](https://github.com/Mazrog/srlzio/tree/master/sample)/[**models.hpp**](https://github.com/Mazrog/srlzio/blob/master/sample/models.hpp)

```C++
//
// Created by Maxime.Cabanal-Duvil on 8/21/2019.
//

#ifndef SRLZIO_MODELS_HPP
#define SRLZIO_MODELS_HPP

#include "../srlzio/srlzio.hpp"

struct Model : xml::node<xml::attributes<int>, xml::leaf<std::string, xml::attributes<>>> {
    Model() {
        attributes.names = { "id" };
        children.names = { "File" };
    }
};

struct ModelBuffer : xml::node<xml::attributes<int>, xml::collection<Model>> {
    ModelBuffer() {
        attributes.names = { "id" };
        children.names = { "Model" };
    }
};

struct Models : xml::node<xml::attributes<>, xml::collection<ModelBuffer>> {
    Models() {
        children.names = { "ModelBuffer" };
    }
};

#endif //SRLZIO_MODELS_HPP
```



### `main.cpp`

[srlzio](https://github.com/Mazrog/srlzio)/[sample](https://github.com/Mazrog/srlzio/tree/master/sample)/[**main.cpp**](https://github.com/Mazrog/srlzio/blob/master/sample/main.cpp)



```C++
#include <iostream>
#include "../srlzio/srlzio.hpp"
#include "models.hpp"

/* --------------------------------------------------------------------------------------- */
int main()
{
	using namespace tinyxml2;
	XMLDocument doc;
	doc.LoadFile("data/recipe.xml");
	XMLElement *root = doc.FirstChildElement("DataFile");

	XMLElement *models_node = root->FirstChildElement("Models");

	Models models;
	parse(models_node, models);

	for (ModelBuffer const &model_buffer : models.children.get<0>().items)
	{
		std::cout << "ModelBuffer: [ID=" << model_buffer.attributes.get<0>() << "]\n";

		for (Model const &model : model_buffer.children.get<0>().items)
		{
			std::cout << "\tModel [ID=" << model.attributes.get<0>() << "] ## " << model.children.names[0] << " -> " << model.children.get<0>().value << "\n";
		}
		std::cout << '\n';
	}

	return 0;
}

```

