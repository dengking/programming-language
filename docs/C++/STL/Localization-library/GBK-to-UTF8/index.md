

# GBK to UTF8



## 使用C++ Localization-library

存在如下问题:

1、`g++` (GCC) 4.8.5 20150623 (Red Hat 4.8.5-28) 不支持 `#include <codecvt>`，无法编译通过

2、在高版本的`g++`中进行编译，如何运行，报了如下 错误:

```C++
terminate called after throwing an instance of 'std::runtime_error'
  what():  locale::facet::_S_create_c_locale name not valid
Aborted (core dumped)

```

### 测试程序

下面是测试程序:

```C++
#include <iostream>
#include <string>

#include <locale>
#include <codecvt>
using namespace std;

class chs_codecvt: public codecvt_byname<wchar_t, char, std::mbstate_t>
{
public:
	chs_codecvt(string s) :
					codecvt_byname(s)
	{

	}
};
class ZJ_Char
{
public:
	static string Utf8ToGbk(string);

	static string GbkToUtf8(string);

	static wstring Utf8ToUnicode(string);

	static wstring GbkToUnicode(string);

	static string UnicodeToUtf8(wstring);

	static string UnicodeToGbk(wstring);

};

//
// Created by zhu on 19-5-22.
//

wstring ZJ_Char::GbkToUnicode(string str)
{
//    codecvt_byname<wchar_t, char, mbstate_t>*dd=;
	wstring_convert<chs_codecvt> gbk(new chs_codecvt("zh_CN.GBK"));    //GBK - whar

	return gbk.from_bytes(str);

}

string ZJ_Char::GbkToUtf8(string str)
{
	return UnicodeToUtf8(GbkToUnicode(str));
}

string ZJ_Char::Utf8ToGbk(string str)
{
	return UnicodeToGbk(Utf8ToUnicode(str));

}
wstring ZJ_Char::Utf8ToUnicode(string str)
{
	wstring ret;

	wstring_convert<codecvt_utf8<wchar_t>> wcv;
	ret = wcv.from_bytes(str);
	return ret;
}

string ZJ_Char::UnicodeToUtf8(wstring wstr)
{
	string ret;
	wstring_convert<codecvt_utf8<wchar_t>> wcv;
	ret = wcv.to_bytes(wstr);
	return ret;
}

string ZJ_Char::UnicodeToGbk(wstring wstr)
{
	wstring_convert<chs_codecvt> gbk(new chs_codecvt("zh_CN.GBK"));    //GBK - whar
	string ret = gbk.to_bytes(wstr);
	return ret;

}
int main()
{
	char *src_str = "葡萄美酒夜光杯";
	cout << "origin string: " << src_str << endl;
	cout << "gbk to utf8: " << ZJ_Char::GbkToUtf8(src_str) << endl;
}
// g++ test.cpp --std=c++11
```

上述code参考自:

1、zhihu [c++1x怎样优雅的读取gbk，转utf-8？](https://www.zhihu.com/question/61139105)

2、csdn [C++ utf-8和gbk互转](https://blog.csdn.net/qq_16778399/article/details/104796938)

### csdn [C++ 编码转换GBK,UTF8,UNICODE](https://blog.csdn.net/u012861467/article/details/106518668)

在介绍如何使用C++11标准库进行中文编码转换之前，先说说一下byte string、multibyte string、wide string之间的区别。

#### byte string

由8比特的字节组成的字符串。由char表示字节。因而: **字符串长度**=**字节数**=**char数**

#### multibyte string

在内存布局上与byte string相同。但是由于它是区域（locale）相关的，所以它有可能表示的是UTF-8, GB18030, EUC-JP, Shift-JIS等格式的字符串，而这些格式中的每个字码（codepoint）可能是由多个连续的char组合构成的，所以: 字符串长度=字码数，但`!=`char数，也`!=`字节数。

> NOTE: `!=`表示不等于

#### wide string

每个宽字符由于操作系统的不同，其宽度为16位或32位的多语言字符代码。

字符编码对应的C类型

unicode(UTF-16LE)   `wchar_t`

UTF-16                       `char16_t`

UTF-32                       `char32_t`

C++标准库中对应关系:

`char *`        `std::string`

`wchar_t*`   `std::wstring`

`char16_t*`  `std::u16string`

`char32_t*`  `std::u32string`

#### 1.string与wstring互相转化

C++标准库从C++11开始提供了`std::codecvt_utf8`和`std::codecvt_byname`两个转换器来完成编码转换，可使用的通用代码如下：

```c++
#include <string>
#include <locale>
#include <codecvt>

// string的编码方式为utf8，则采用：
std::string wstring2utf8string(const std::wstring &str)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
	return strCnv.to_bytes(str);
}

std::wstring utf8string2wstring(const std::string &str)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
	return strCnv.from_bytes(str);
}

// string的编码方式为除utf8外的其它编码方式，可采用：
std::string wstring2string(const std::wstring &str, const std::string &locale)
{
	typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> F;
	static std::wstring_convert<F> strCnv(new F(locale));

	return strCnv.to_bytes(str);
}

std::wstring string2wstring(const std::string &str, const std::string &locale)
{
	typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> F;
	static std::wstring_convert<F> strCnv(new F(locale));

	return strCnv.from_bytes(str);
}


```

#### `locale`可取值

如果是GBK string与wstring互相转化，`locale`可取值：

linux下：

`zh_CN.GBK`

`zh_CN.GB2312`

`zh_CN.GB18030`

windows下：

标准格式的locale：

`Chinese_China.936`

`zh-CN`

`.936`

非标准格式的`locale`：

`chs`

`Chinese-simplified`

`Chinese`

`ZHI`

不能使用的locale：

`Chinese.936`，`chs.936`，`Chinese.GB2312`，`chs.GB18030`等此类值。

下面通过一些例子说明上面的函数如何使用吧。

#### 2.string与wstring如何输出到控制台





### github [conversion_gbk_utf8](https://github.com/qq1134993111/conversion_gbk_utf8)



## 使用boost locale

### stackoverflow [How to convert the word '' from utf-8 to gbk](https://stackoverflow.com/questions/43871458/how-to-convert-the-word-from-utf-8-to-gbk)

[A](https://stackoverflow.com/a/43911015)



```C++
#include <fstream>
#include <iostream>
#include <boost/locale.hpp>

int main() {
    boost::locale::generator gen;
    auto CN = gen.generate("zh_CN.GBK");

    for (std::string const input : {
        std::string { static_cast<char>(0xE4), static_cast<char>(0xB6), static_cast<char>(0xAE), 0 },
        std::string { static_cast<char>(0xEE), static_cast<char>(0xA1), static_cast<char>(0xA3), 0 },
    })
    {
        std::ofstream o("1.dat", std::ios::binary);
        o.imbue(CN);

        try {
            o << boost::locale::conv::from_utf(input, "GBK", boost::locale::conv::skip) << "\n";
            std::cout << "Conversion ok!\n";
        }
        catch(boost::locale::conv::conversion_error e){
            std::cout << e.what() << "\n";
        }
    }
}
// g++ test.cpp -I. --std=c++11

```

> NOTE: 
>
> 1、上述code，链接报错:
>
> ```C++
> /tmp/cc6a14Hp.o：在函数‘main’中：
> test.cpp:(.text+0x1f)：对‘boost::locale::generator::generator()’未定义的引用
> test.cpp:(.text+0x69)：对‘boost::locale::generator::generate(std::string const&) const’未定义的引用
> test.cpp:(.text+0x2e4)：对‘boost::locale::generator::~generator()’未定义的引用
> test.cpp:(.text+0x48e)：对‘boost::locale::generator::~generator()’未定义的引用
> /tmp/cc6a14Hp.o：在函数‘std::string boost::locale::conv::from_utf<char>(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, std::string const&, boost::locale::conv::method_type)’中：
> test.cpp:(.text._ZN5boost6locale4conv8from_utfIcEESsRKSbIT_St11char_traitsIS3_ESaIS3_EERKSsNS1_11method_typeE[_ZN5boost6locale4conv8from_utfIcEESsRKSbIT_St11char_traitsIS3_ESaIS3_EERKSsNS1_11method_typeE]+0x5d)：对‘std::string boost::locale::conv::from_utf<char>(char const*, char const*, std::string const&, boost::locale::conv::method_type)’未定义的引用
> collect2: 错误：ld 返回 1
> ```
>
> 



## 使用system call

弊端:

1、需要考虑cross plateform

### 素材

1、iteye [c中实现utf8和gbk的互转](https://www.iteye.com/blog/jimmee-2174693)

2、csdn [C++实现utf8和gbk编码字符串互相转换](https://blog.csdn.net/u012234115/article/details/83186386)

> NOTE: 参见 `C++实现utf8和gbk编码字符串互相转换` 章节

3、zhidao [用C/C++写一个字符串GBK转UTF-8编码的函数，并写main函数测试(在线等)](https://zhidao.baidu.com/question/103273521.html)

4、csdn [linux下c++编程 转码 GBKTOUTF8问题](https://bbs.csdn.net/topics/320083099)



## TODO

microsoft [[C++]Unicode Encoding Conversions with STL Strings and Win32 APIs](https://docs.microsoft.com/en-us/archive/msdn-magazine/2016/september/c-unicode-encoding-conversions-with-stl-strings-and-win32-apis)