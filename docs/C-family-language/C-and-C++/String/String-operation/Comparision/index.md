# String comparison



## C++ Case-insensitive string comparison

### stackoverflow [Case-insensitive string comparison in C++ [closed]](https://stackoverflow.com/questions/11635/case-insensitive-string-comparison-in-c)



#### [A](https://stackoverflow.com/a/315463) `boost::iequals`

Boost includes a handy algorithm for this:

```cpp
#include <boost/algorithm/string.hpp>
// Or, for fewer header dependencies:
//#include <boost/algorithm/string/predicate.hpp>

std::string str1 = "hello, world!";
std::string str2 = "HELLO, WORLD!";

if (boost::iequals(str1, str2))
{
    // Strings are identical
}
```

#### [A](https://stackoverflow.com/a/2886589) `std::char_traits`

Take advantage of the standard `char_traits`. Recall that a `std::string` is in fact a typedef for `std::basic_string<char>`, or more explicitly, `std::basic_string<char, std::char_traits<char> >`. The `char_traits` type describes how characters compare, how they copy, how they cast etc. All you need to do is typedef a new string over `basic_string`, and provide it with your own custom `char_traits` that compare case insensitively.

```cpp
#include <string>
#include <cctype>
#include <iostream>
struct ci_char_traits: public std::char_traits<char>
{
	static bool eq(char c1, char c2)
	{
		return std::toupper(c1) == std::toupper(c2);
	}
	static bool ne(char c1, char c2)
	{
		return std::toupper(c1) != std::toupper(c2);
	}
	static bool lt(char c1, char c2)
	{
		return std::toupper(c1) < std::toupper(c2);
	}
	static int compare(const char *s1, const char *s2, std::size_t n)
	{
		while (n-- != 0)
		{
			if (std::toupper(*s1) < std::toupper(*s2))
				return -1;
			if (std::toupper(*s1) > std::toupper(*s2))
				return 1;
			++s1;
			++s2;
		}
		return 0;
	}
	static const char* find(const char *s, int n, char a)
	{
		while (n-- > 0 && std::toupper(*s) != std::toupper(a))
		{
			++s;
		}
		return s;
	}
};

typedef std::basic_string<char, ci_char_traits> ci_string;

int main()
{
	ci_string s = "hello";
	ci_string s1 = "HELLO";
	if (s == s1)
	{
		std::cout << "equal" << std::endl;
	}
	else
	{
		std::cout << "not equal" << std::endl;
	}
}
// g++ test.cpp

```

> NOTE: 输出如下:
>
> ```
> equal
> ```
>
> 

The details are on [Guru of The Week number 29](http://www.gotw.ca/gotw/029.htm).



#### [A](https://stackoverflow.com/a/4119881) custom implementation

The trouble with boost is that you have to link with and depend on boost. Not easy in some cases (e.g. android).

And using char_traits means *all* your comparisons are case insensitive, which isn't usually what you want.

This should suffice. It should be reasonably efficient. Doesn't handle unicode or anything though.

```cpp
#include <string>
#include <cctype>
#include <iostream>
bool iequals(const std::string &a, const std::string &b)
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (std::tolower(a[i]) != std::tolower(b[i]))
			return false;
	return true;
}

int main()
{
	std::string s = "hello";
	std::string s1 = "HELLO";
	if (iequals(s, s1))
	{
		std::cout << "equal" << std::endl;
	}
	else
	{
		std::cout << "not equal" << std::endl;
	}
}
// g++ test.cpp

```

Update: Bonus C++14 version (`#include <algorithm>`):

```cpp
#include <string>
#include <cctype>
#include <iostream>
#include <algorithm>

bool iequals(const std::string& a, const std::string& b)
{
    return std::equal(a.begin(), a.end(), b.begin(),
                      [](char a, char b) {
                          return std::tolower(a) == std::tolower(b);
                      });
}

int main()
{
	std::string s = "hello";
	std::string s1 = "HELLO";
	if (iequals(s, s1))
	{
		std::cout << "equal" << std::endl;
	}
	else
	{
		std::cout << "not equal" << std::endl;
	}
}
// g++ --std=c++11 test.cpp

```



#### [A](https://stackoverflow.com/a/11685) [strcasecmp](http://www.opengroup.org/onlinepubs/009695399/functions/strcasecmp.html)

If you are on a POSIX system, you can use [strcasecmp](http://www.opengroup.org/onlinepubs/009695399/functions/strcasecmp.html). This function is not part of standard C, though, nor is it available on Windows. This will perform a case-insensitive comparison on 8-bit chars, so long as the locale is POSIX. If the locale is not POSIX, the results are undefined (so it might do a localized compare, or it might not). A wide-character equivalent is not available.

Failing that, a large number of historic C library implementations have the functions stricmp() and strnicmp(). Visual C++ on Windows renamed all of these by prefixing them with an underscore because they aren’t part of the ANSI standard, so on that system they’re called [_stricmp or _strnicmp](http://msdn.microsoft.com/en-us/library/k59z8dwe.aspx). Some libraries may also have wide-character or multibyte equivalent functions (typically named e.g. wcsicmp, mbcsicmp and so on).

C and C++ are both largely ignorant of internationalization issues, so there's no good solution to this problem, except to use a third-party library. Check out [IBM ICU (International Components for Unicode)](http://www.icu-project.org/) if you need a robust library for C/C++. ICU is for both Windows and Unix systems.



## C Case-insensitive string comparison

### stackoverflow [Case Insensitive String comp in C](https://stackoverflow.com/questions/5820810/case-insensitive-string-comp-in-c)

```C++
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int strcicmp(char const *a, char const *b)
{
	if (strlen(a) != strlen(b))
	{
		return 1;
	}
	int diff = 0;
	for (; *a; a++, b++)
	{
		diff = tolower((unsigned char) *a) - tolower((unsigned char) *b);
		if (diff != 0)
			return diff;
	}
	return diff;
}

int main()
{
	const char *s = "hello";
	const char *s1 = "HELLOo";
	if (strcicmp(s, s1) == 0)
	{
		printf("equal\n");
	}
	else
	{
		printf("not equal\n");
	}
}
// gcc  test.c


```

