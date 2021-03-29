# csdn [C++实现utf8和gbk编码字符串互相转换](https://blog.csdn.net/u012234115/article/details/83186386)

不同系统或者服务器之间消息传递经常遇到编码转换问题，这里用C++实现了一个轻量的gbk和utf8互相转换，可跨平台使用。（重量级的可以用libiconv库）

在windows下用`<windows.h>`头文件里的函数进行多字节和宽字符转换，linux下采用<iconv.h>头文件里的函数进行编码互相解析。


```c++
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

#ifdef _WIN32
#include <windows.h>

string GbkToUtf8(const char *src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

string Utf8ToGbk(const char *src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}
#else
#include <iconv.h>

int GbkToUtf8(char *str_str, size_t src_len, char *dst_str, size_t dst_len)
{
	iconv_t cd;
	char **pin = &str_str;
	char **pout = &dst_str;

	cd = iconv_open("utf8", "gbk");
	if (cd == 0)
		return -1;
	memset(dst_str, 0, dst_len);
	if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
		return -1;
	iconv_close(cd);
	*pout = '\0';

	return 0;
}

int Utf8ToGbk(char *src_str, size_t src_len, char *dst_str, size_t dst_len)
{
	iconv_t cd;
	char **pin = &src_str;
	char **pout = &dst_str;

	cd = iconv_open("gbk", "utf8");
	if (cd == 0)
		return -1;
	memset(dst_str, 0, dst_len);
	if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
		return -1;
	iconv_close(cd);
	*pout = '\0';

	return 0;
}

#endif

int main(void)
{
	char *src_str = "葡萄美酒夜光杯";
	cout << "origin string: " << src_str << endl;

#ifdef _WIN32
	// windows default is gbk
	string dst_str = GbkToUtf8(src_str);
	cout << "gbk to utf8: " << dst_str << endl;

	string str_utf8 = Utf8ToGbk(dst_str.c_str());
	cout << "utf8 to gbk: " << str_utf8 << endl;
#else
	// unix default is utf8
	char dst_gbk[1024] = { 0 };
	Utf8ToGbk(src_str, strlen(src_str), dst_gbk, sizeof(dst_gbk));
	cout << "utf8 to gbk: " << dst_gbk << endl;

	char dst_utf8[1024] = { 0 };
	GbkToUtf8(dst_gbk, strlen(dst_gbk), dst_utf8, sizeof(dst_utf8));
	cout << "gbk to utf8: " << dst_utf8 << endl;
#endif

	return 0;
}
// g++ test.cpp

```



> NOTE: 
>
> 1、编译报错如下:
>
> ```C++
> test.cpp: In function 'int GbkToUtf8(char*, size_t, char*, size_t)':
> test.cpp:58:10: error: invalid conversion from 'char' to 'char*' [-fpermissive]
>   *pout = '\0';
>           ^~~~
> test.cpp: In function 'int Utf8ToGbk(char*, size_t, char*, size_t)':
> test.cpp:76:10: error: invalid conversion from 'char' to 'char*' [-fpermissive]
>   *pout = '\0';
>           ^~~~
> test.cpp: In function 'int main()':
> test.cpp:85:18: warning: ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]
>   char *src_str = "葡萄美酒夜光杯";
>                   ^~~~~~~~~~~~~~~~
> 
> ```
>
> 



## 修正的程序

```C++
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

#ifdef _WIN32
#include <windows.h>

string GbkToUtf8(const char *src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

string Utf8ToGbk(const char *src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}
#else
#include <iconv.h>

int GbkToUtf8(char *str_str, size_t src_len, char *dst_str, size_t dst_len)
{
	iconv_t cd;
	char **pin = &str_str;
	char **pout = &dst_str;

	cd = iconv_open("utf8", "gbk");
	if (cd == 0)
		return -1;
	memset(dst_str, 0, dst_len);
	if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
		return -1;
	iconv_close(cd);
	*pout = 0;

	return 0;
}

int Utf8ToGbk(char *src_str, size_t src_len, char *dst_str, size_t dst_len)
{
	iconv_t cd;
	char **pin = &src_str;
	char **pout = &dst_str;

	cd = iconv_open("gbk", "utf8");
	if (cd == 0)
		return -1;
	memset(dst_str, 0, dst_len);
	if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
		return -1;
	iconv_close(cd);
	*pout = 0;

	return 0;
}

#endif

int main(void)
{
	char *src_str = "葡萄美酒夜光杯";
	cout << "origin string: " << src_str << endl;

#ifdef _WIN32
	// windows default is gbk
	string dst_str = GbkToUtf8(src_str);
	cout << "gbk to utf8: " << dst_str << endl;

	string str_utf8 = Utf8ToGbk(dst_str.c_str());
	cout << "utf8 to gbk: " << str_utf8 << endl;
#else
	// unix default is utf8
	char dst_gbk[1024] = { 0 };
	Utf8ToGbk(src_str, strlen(src_str), dst_gbk, sizeof(dst_gbk));
	cout << "utf8 to gbk: " << dst_gbk << endl;

	char dst_utf8[1024] = { 0 };
	GbkToUtf8(dst_gbk, strlen(dst_gbk), dst_utf8, sizeof(dst_utf8));
	cout << "gbk to utf8: " << dst_utf8 << endl;
#endif

	return 0;
}


```



## 优化后的程序

```C++
/**
 * @brief
 * 参考自:
 * http://www.voidcn.com/article/p-ockzffzf-bmh.html
 * https://blog.csdn.net/u012234115/article/details/83186386
 *
 * @param src_str
 * @param src_len
 * @param dst_str
 * @param dst_len
 * @return
 */
int GbkToUtf8(const char *src_str, size_t src_len, char *dst_str, size_t dst_len)
{
	/**
	 * 源字符串为空，不需要进行转换
	 */
	if (src_str[0] == '\0' || src_len == 0)
	{
		return 0;
	}
	/**
	 * 目标字符串为空，则无法进行转换
	 */
	if (dst_len == 0)
	{
		LOG_ERROR("目标字符串长度为0，无法进行转换");
		return -1;
	}
	//计算由gbk转换为unicode后，unicode编码的长度
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0); //cp_acp指示了转换为unicode编码的编码类型
	if (len <= 0)
	{
		LOG_ERROR("转换失败，长度计算错误");
		return -1;
	}
	else
	{
		std::vector<wchar_t> w_str_(len + 1, 0); // 使用vector，会更加安全高效
		wchar_t *wstr = w_str_.data();
		//ansi到unicode转换
		MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len); //cp_acp指示了转换为unicode编码的编码类型

		// //计算unicode转换为utf8后，utf8编码的长度
		len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		if (len <= 0)
		{
			LOG_ERROR("转换失败，长度计算错误");
			return -1;
		}
		else
		{
//			std::vector<char> str_(len + 1, 0); // 使用vector，会更加安全高效
//			char *str = str_.data();
			if (len + 1 > dst_len)
			{
				LOG_ERROR("长度大于目标字符串数组的长度，信息可能被截断");
				return -1;
			}
			else
			{
				//unicode到utf8转换
				WideCharToMultiByte(CP_UTF8, 0, wstr, -1, dst_str, len, NULL, NULL); //cp_utf8指示了unicode转换为的类型
				return 0;
			}
		}
	}

}

std::string Utf8ToGbk(const char *src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
//	wchar_t* wszGBK = new wchar_t[len + 1];
//	memset(wszGBK, 0, len * 2 + 2);
	std::vector<wchar_t> wszGBK_(len + 1, 0); // 使用vector，会更加安全高效
	wchar_t *wszGBK = wszGBK_.data();

	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);

//	char* szGBK = new char[len + 1];
//	memset(szGBK, 0, len + 1);
	std::vector<char> szGBK_(len + 1, 0); // 使用vector，会更加安全高效
	char *szGBK = szGBK_.data();

	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);

//	if (wszGBK) delete[] wszGBK;
//	if (szGBK) delete[] szGBK;
	return strTemp;
}
}
#else
#include <iconv.h>

namespace AMUST
{
/**
 * @brief 将GBK转换为UTF8
 *
 * @param str_str
 * @param src_len
 * @param dst_str
 * @param dst_len
 * @return
 */
static int GbkToUtf8(char *str_str, size_t src_len, char *dst_str, size_t dst_len)
{
	/**
	 * 源字符串为空，不需要进行转换
	 */
	if (src_str[0] == '\0' || src_len == 0)
	{
		return 0;
	}
	/**
	 * 目标字符串为空，则无法进行转换
	 */
	if (dst_len == 0)
	{
		return -1;
	}
	iconv_t cd;
	char **pin = &str_str;
	char **pout = &dst_str;

	cd = iconv_open("utf8", "gbk");
	if (cd == 0) // 分配失败了
	{
		return -1;
	}
	SCOPE_EXIT { iconv_close(cd);}; // 保证资源得到释放
	memset(dst_str, 0, dst_len);
	if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
	{
		// 转换失败了
		return -1;
	}

	*pout = 0;

	return 0;
}

static int Utf8ToGbk(char *src_str, size_t src_len, char *dst_str, size_t dst_len)
{
	/**
	 * 源字符串为空，不需要进行转换
	 */
	if (src_str[0] == '\0' || src_len == 0)
	{
		return 0;
	}
	/**
	 * 目标字符串为空，则无法进行转换
	 */
	if (dst_len == 0)
	{
		return -1;
	}
	iconv_t cd;
	char **pin = &src_str;
	char **pout = &dst_str;

	cd = iconv_open("gbk", "utf8");
	if (cd == 0)
	{
		return -1;
	}
	SCOPE_EXIT { iconv_close(cd);}; // 保证资源得到释放

	memset(dst_str, 0, dst_len);
	if (iconv(cd, pin, &src_len, pout, &dst_len) == -1)
	{
		return -1;
	}
	*pout = 0;

	return 0;
}
```



Windows版的注释是参考的: 

http://www.voidcn.com/article/p-ockzffzf-bmh.html