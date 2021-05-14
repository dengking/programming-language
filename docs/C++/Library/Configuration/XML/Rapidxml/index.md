# Rapidxml



## 官网 [RapidXML](http://rapidxml.sourceforge.net/)

You may also like to check [Boost.PropertyTree](http://boost.org/) library, which presents a higher level interface, and uses RapidXml as its default XML parser.

> NOTE: `boost\property_tree\detail`就用了`rapidxml.hpp。

### [RAPIDXML Manual](http://rapidxml.sourceforge.net/manual.html)

**1.4 Memory Allocation**

RapidXml uses a special memory pool object to allocate nodes and attributes, because direct allocation using `new` operator would be far too slow. Underlying memory allocations performed by the pool can be customized by use of [memory_pool::set_allocator()](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool_c0a55a6ef0837dca67572e357100d78a_1c0a55a6ef0837dca67572e357100d78a) function. See class [memory_pool](http://rapidxml.sourceforge.net/manual.html#classrapidxml_1_1memory__pool) for more information.

> NOTE: 
>
> 1、值得借鉴，函数原型:
>
> ```C++
> void set_allocator(alloc_func *af, free_func *ff);
> ```
>
> 约定接口





## Usage

### stackoverflow [How to parse an XML file with RapidXml](https://stackoverflow.com/questions/2808022/how-to-parse-an-xml-file-with-rapidxml)



### Example

```C++
<?xml  version="1.0" encoding="GBK"?>
<config>
	<markets>
		<market mktname="中金所期权" mkttype="0x1103"/>
	</markets>
    <indexes>
        <index indexname="沪深300指数" mkttype="0x101" code="1B0300"/>
    </indexes>
</config>
```



```C++
#include "rapidxml_utils.hpp"
#include "iostream"

int main()
{
	rapidxml::file<> xmlFile("test.xml"); // Default template is char
	rapidxml::xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

	rapidxml::xml_node<> *root = doc.first_node("config");
	rapidxml::xml_node<> *markets = root->first_node("markets");
	std::cout << markets->name() << "\n";
	rapidxml::xml_node<> *indexes = root->first_node("indexes");
	std::cout << indexes->name() << "\n";

	for (rapidxml::xml_node<> *node = markets->first_node(); node; node = node->next_sibling())
	{
		std::cout << node->name() << "\n";
		for (rapidxml::xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
		{
			std::cout << attr->name() << "\n";
			std::cout << attr->value() << "\n";
		}

	}
}

```



### cnblogs [C++中rapidxml用法及例子](https://www.cnblogs.com/sevenyuan/p/3706654.html)