# Algorithm comma join

## stackoverflow [Convert vector to delimited string](https://stackoverflow.com/questions/20817322/convert-vectorint-to-delimited-string)



### [A](https://stackoverflow.com/a/20817355)

#### `std::copy` + `std::ostream_iterator`

```C++
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <iostream>

int main()
{
	std::vector<int> data = { 42, 1, 2, 3, 4, 5 };

	std::ostringstream oss;
	std::copy(data.begin(), data.end(), std::ostream_iterator<int>(oss, ";"));

	std::string result(oss.str());
	std::cout << result << "\n";
}
// g++ test.cpp --std=c++11 -pedantic -Wall

```

输出如下:

```C++
42;1;2;3;4;5;
```

N.B. In C++11, you can use the more general form

```cpp
using std::begin;
using std::end;
std::copy(begin(data), end(data), std::ostream_iterator<int>(oss, ";"));
```

Where the using-declarations are not required if ADL can be used (like in the example above).

> NOTE: 
>
> 完整测试程序:
>
> ```C++
> #include <sstream>
> #include <string>
> #include <vector>
> #include <iterator>
> #include <iostream>
> 
> int main()
> {
> 	std::vector<int> data = { 42, 1, 2, 3, 4, 5 };
> 	using std::begin;
> 	using std::end;
> 
> 	std::ostringstream oss;
> 	std::copy(begin(data), end(data), std::ostream_iterator<int>(oss, ";"));
> 
> 	std::string result(oss.str());
> 	std::cout << result << "\n";
> }
> // g++ test.cpp --std=c++11 -pedantic -Wall
> 
> ```
>
> 

#### `std::accumulate`

> NOTE: 
>
> 1、这种方式是效率较低的

Also possible, but maybe a bit less efficient:

```cpp
std::string s;
for(auto const& e : v) s += std::to_string(e) + ";";
```

which can be written via `std::accumulate` in `<algorithm>` as:

```cpp
std::string s = std::accumulate(begin(v), end(v), std::string{},
    [](std::string r, int p){ return std::move(r) + std::to_string(p) + ";"; });
```

(IIRC there was some method to eliminate the copying, maybe by taking the lambda-parameter by reference `std::string& r`.)

#### w/o the trailing semi-colon

A version w/o the trailing semi-colon (thanks to [Dietmar Kühl](https://stackoverflow.com/users/1120273/dietmar-kuhl)):

```C++
std::vector<int> data = {42, 1, 2, 3, 4, 5};

std::ostringstream out;
if (!v.empty())
{
    std::copy(v.begin(), v.end() - 1, std::ostream_iterator<int>(out, ";"));
    out << v.back();
}

std::string result( out.str() );
std::cout << result << "\n";

```



> NOTE: 
>
> 完整测试程序:
>
> ```C++
> #include <sstream>
> #include <string>
> #include <vector>
> #include <iterator>
> #include <iostream>
> 
> int main()
> {
> 	std::vector<int> v = { 42, 1, 2, 3, 4, 5 };
> 
> 	std::ostringstream out;
> 	if (!v.empty())
> 	{
> 		std::copy(v.begin(), v.end() - 1, std::ostream_iterator<int>(out, ";"));
> 		out << v.back();
> 	}
> 
> 	std::string result(out.str());
> 	std::cout << result << "\n";
> }
> // g++ test.cpp --std=c++11 -pedantic -Wall
> 
> ```
>
> 



## stackoverflow [Printing lists with commas C++](https://stackoverflow.com/questions/3496982/printing-lists-with-commas-c)



### [A](https://stackoverflow.com/a/3497021)

> NOTE: 
>
> 1、这个回答的作者并没有给出解释，在stackoverflow [std::ostream_iterator prevent last item from using the delimiter [duplicate]](https://stackoverflow.com/questions/17355864/stdostream-iterator-prevent-last-item-from-using-the-delimiter) # [A](https://stackoverflow.com/a/17358207) 中给出的回答可以给出理解下面code的关键，下面收录了。
>
> 

```C++
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <iostream>

// infix_iterator.h
//
// Lifted from Jerry Coffin's 's prefix_ostream_iterator
#if !defined(INFIX_ITERATOR_H_)
#define  INFIX_ITERATOR_H_
#include <ostream>
#include <iterator>
template<class T, class charT = char, class traits = std::char_traits<charT> >
class infix_ostream_iterator: public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	std::basic_ostream<charT, traits> *os; // 输出流
	charT const *delimiter; // 分隔符
	bool first_elem; // 是否是第一个元素的标志
public:
	typedef charT char_type;
	typedef traits traits_type;
	typedef std::basic_ostream<charT, traits> ostream_type;

	infix_ostream_iterator(ostream_type &s) :
					os(&s), delimiter(0), first_elem(true)
	{
	}
	infix_ostream_iterator(ostream_type &s, charT const *d) :
					os(&s), delimiter(d), first_elem(true)
	{
	}
	infix_ostream_iterator<T, charT, traits>& operator=(T const &item)
	{
		// Here's the only real change from ostream_iterator:
		// Normally, the '*os << item;' would come before the 'if'.
		if (!first_elem && delimiter != 0)
			*os << delimiter;
		*os << item;
		first_elem = false;
		return *this;
	}
	infix_ostream_iterator<T, charT, traits>& operator*()
	{
		return *this;
	}
	infix_ostream_iterator<T, charT, traits>& operator++()
	{
		return *this;
	}
	infix_ostream_iterator<T, charT, traits>& operator++(int)
	{
		return *this;
	}
};
#endif

int main()
{
	std::string keywords("hello world");

	std::ostringstream out;
	// ...
	std::copy(keywords.begin(), keywords.end(), infix_ostream_iterator<char>(out, ","));
	std::cout << out.str() << std::endl;
}
// g++ test.cpp --std=c++11 -pedantic -Wall

```

输出如下:

```C++
h,e,l,l,o, ,w,o,r,l,d
```



### [A](https://stackoverflow.com/a/35998265)

In an experimental C++17 ready compiler coming soon to you, you can use [`std::experimental::ostream_joiner`](http://en.cppreference.com/w/cpp/experimental/ostream_joiner):

```cpp
#include <algorithm>
#include <experimental/iterator>
#include <iostream>
#include <iterator>

int main()
{
    int i[] = {1, 2, 3, 4, 5};
    std::copy(std::begin(i),
              std::end(i),
              std::experimental::make_ostream_joiner(std::cout, ", "));
}
```

Live examples using [**GCC 6.0 SVN**](http://melpon.org/wandbox/permlink/0nf3Oh6gvrrQGxhY) and [**Clang 3.9 SVN**](http://melpon.org/wandbox/permlink/04ZzdkPe5QmKc55E)



## stackoverflow [std::ostream_iterator prevent last item from using the delimiter [duplicate]](https://stackoverflow.com/questions/17355864/stdostream-iterator-prevent-last-item-from-using-the-delimiter)



### [A](https://stackoverflow.com/a/17358207)

`copy` could be implement as:

```cpp
template<class InputIterator, class OutputIterator>
OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result)
{
  while (first!=last) {
  *result = *first;
  ++result; ++first;
  }
  return result;
}
```

The assignment to the `ostream_iterator` (output iterator) could be implemented as:

```cpp
ostream_iterator<T,charT,traits>& operator= (const T& value) {
  *out_stream << value;
  if (delim!=0) *out_stream << delim;
  return *this;
}
```

So the delimiter will be appended on every assignment to the output iterator. To avoid the delimiter being appended to the last vector element, the last element should be assigned to an output iterator without delimiter, for example:

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
int main() {
  std::vector<int> ints = {10,20,30,40,50,60,70,80,90};
  std::copy(ints.begin(), ints.end()-1, std::ostream_iterator<int>(std::cout, ","));
  std::copy(ints.end()-1, ints.end(), std::ostream_iterator<int>(std::cout));
  std::cout << std::endl;
  return 0;
}
```

Results in:

```cpp
10,20,30,40,50,60,70,80,90
```