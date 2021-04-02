# Stream operation

本文对stream的operation进行总结，其实这些operation所对应的就是stream的member function，stream包括:

| stream        | class                                                        | buffer area |
| ------------- | ------------------------------------------------------------ | ----------- |
| input stream  | [std::basic_istream](https://en.cppreference.com/w/cpp/io/basic_istream) | get area    |
| output stream | [std::basic_ostream](https://en.cppreference.com/w/cpp/io/basic_ostream) | put area    |

关于buffer area，参见`./Buffe`。



下面通过对它们的member function进行对比来进行学习。

## Positioning operation

> NOTE: member function的名称中的后缀对应的是buffer area。

|                                                              | **set position indicator**                                   | **return position indicator**                                | buffer area |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ----------- |
| [std::basic_istream](https://en.cppreference.com/w/cpp/io/basic_istream) | [seekg](https://en.cppreference.com/w/cpp/io/basic_istream/seekg) | [tellg](https://en.cppreference.com/w/cpp/io/basic_istream/tellg) | get area    |
| [std::basic_ostream](https://en.cppreference.com/w/cpp/io/basic_ostream) | [seekp](https://en.cppreference.com/w/cpp/io/basic_ostream/seekp) | [tellp](https://en.cppreference.com/w/cpp/io/basic_ostream/tellp) | put area    |

需要重点学习的是  **set position indicator**类方法，

| prototype                                                    |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| `basic_ostream& seek*( pos_type pos );`                      | sets the position indicator to absolute (relative to the beginning of the file) value `pos` |
| `basic_ostream& seek*( off_type off,` [std::ios_base::seekdir](http://en.cppreference.com/w/cpp/io/ios_base/seekdir) `dir );` | sets the position indicator to offset `off` relative to `dir` |



### Example

来源: cppreference [std::basic_ostream<CharT,Traits>::seekp](https://en.cppreference.com/w/cpp/io/basic_ostream/seekp)

```C++
#include <sstream>
#include <iostream>

int main()
{
	std::ostringstream os("hello, world");
	os.seekp(7);
	os << 'W';
	os.seekp(0, std::ios_base::end);
	os << '!';
	os.seekp(0);
	os << 'H';
	std::cout << os.str() << '\n';
}
// g++ test.cpp

```

输出:

```
Hello, World!
```



stackoverflow [remove char from stringstream and append some data](https://stackoverflow.com/questions/4546021/remove-char-from-stringstream-and-append-some-data)

> NOTE: 这种需求是我们经常会碰到的





### Example: 从`std::stringstream`中将最后一个元素剔除: 

```C++
	static std::string Create(const char *TableName, const CStructRtti *StructRtti)
	{
		std::stringstream S;
		S << "CREATE TABLE IF NOT EXISTS " << TableName << " (";
		int FieldCount = StructRtti->GetFieldCount();
		for (const SFieldInfo &FieldInfo : StructRtti->GetFieldInfoList())
		{
			S << FieldInfo.m_sFieldName << " " << GetColunmType(FieldInfo.m_Type) << " ,";
		}
		S.seekp(-1, std::ios_base::end); // 剔除最后一个元素
		S << ")";
		return S.str();
	}
```

