# Operation: `values` like python dict



## thispointer [How to copy all Values from a Map to a Vector in C++](https://thispointer.com/how-to-copy-all-values-from-a-map-to-a-vector-in-c/)

```C++
// Map of string & int
// To store the frequency count of words
std::map<std::string, int> wordMap = { { "is", 6 }, { "the", 5 }, { "hat", 9 }, { "at", 6 }, { "of", 2 }, { "hello", 1 } };

```

### Copy all values from a map to vector using Range Based for Loop

```C++
#include <string>
#include <map>
#include <vector>
#include <iterator>     // std::ostream_iterator
#include <iostream>
#include <algorithm>

int main()
{
	std::vector<int> vecOfValues;
	// Map of string & int
	// To store the frequency count of words
	std::map<std::string, int> wordMap = { { "is", 6 }, { "the", 5 }, { "hat", 9 }, { "at", 6 }, { "of", 2 }, { "hello", 1 } };
	/*** Copy all value fields from map to a vector using Range Based For Loop ***/
	for (auto elem : wordMap)
		vecOfValues.push_back(elem.second);
	// dump the list
	std::copy(vecOfValues.begin(), vecOfValues.end(), std::ostream_iterator<int>(std::cout, "\n"));
}
// g++ --std=c++11 test.cpp

```

### Copy all values from a map to vector using for_each() & Lambda function

```C++
#include <string>
#include <map>
#include <vector>
#include <iterator>     // std::ostream_iterator
#include <iostream>
#include <algorithm>

int main()
{
	// Map of string & int
	// To store the frequency count of words
	std::map<std::string, int> wordMap = { { "is", 6 }, { "the", 5 }, { "hat", 9 }, { "at", 6 }, { "of", 2 }, { "hello", 1 } };
	std::vector<int> vecOfValues;
	vecOfValues.reserve(wordMap.size());

	/*** Copy all value fields from map to a vector using Lambda function ***/
	std::for_each(wordMap.begin(), wordMap.end(), [&](std::pair<const std::string, int> &element)
	{
		vecOfValues.push_back(element.second);
	});

	// dump the list
	std::copy(vecOfValues.begin(), vecOfValues.end(), std::ostream_iterator<int>(std::cout, "\n"));
}
// g++ --std=c++11 test.cpp

```

### Copy all values from a map to vector using transform() and lambda function

```C++
#include <string>
#include <map>
#include <vector>
#include <iterator>     // std::ostream_iterator
#include <iostream>
#include <algorithm>

int main()
{
	// Map of string & int
	// To store the frequency count of words
	std::map<std::string, int> wordMap = { { "is", 6 }, { "the", 5 }, { "hat", 9 }, { "at", 6 }, { "of", 2 }, { "hello", 1 } };
	std::vector<int> vecOfValues;
	vecOfValues.reserve(wordMap.size());

	/*** Copy all value fields from map to a vector using transform() & Lambda function ***/
	std::transform(wordMap.begin(), wordMap.end(), std::back_inserter(vecOfValues), [](std::pair<std::string, int> const &pair)
	{
		return pair.second;
	});

	// dump the list
	std::copy(vecOfValues.begin(), vecOfValues.end(), std::ostream_iterator<int>(std::cout, "\n"));
}
// g++ --std=c++11 test.cpp

```

### Copy all values from a map to vector using transform() & function pointer

```C++
#include <string>
#include <map>
#include <vector>
#include <iterator>     // std::ostream_iterator
#include <iostream>
#include <algorithm>

/*
 * Template function that returns 2nd Value from a pair
 */
template<typename K, typename V>
V getSecond(std::pair<K, V> const &pair)
{
	return pair.second;
}

int main()
{
	// Map of string & int
	// To store the frequency count of words
	std::map<std::string, int> wordMap = { { "is", 6 }, { "the", 5 }, { "hat", 9 }, { "at", 6 }, { "of", 2 }, { "hello", 1 } };
	std::vector<int> vecOfValues;
	vecOfValues.reserve(wordMap.size());

	/*** Copy all values from a map to vector using transform() & function pointer ***/
	std::transform(wordMap.begin(), wordMap.end(), std::back_inserter(vecOfValues), &getSecond<std::string, int>);

	// dump the list
	std::copy(vecOfValues.begin(), vecOfValues.end(), std::ostream_iterator<int>(std::cout, "\n"));
}
// g++ --std=c++11 test.cpp

```



## stackoverflow [Getting a list of values from a map](https://stackoverflow.com/questions/4195611/getting-a-list-of-values-from-a-map)

[A](https://stackoverflow.com/a/4195743)

A `map` element is defined as a `map::value_type`, and the type of it is a `pair<A,B>`. `first` is the key and `second` is the value. You can write a [functor](http://en.wikipedia.org/wiki/Function_object) to extract `second` from a `value_type`, and copy that in to a `vector` (or a `list`, or whatever you want.) The best way to do the copying is to use `transform`, which does just what its name implies: it takes a value of one type and transforms it to a different type of value.

Here's a complete working example:

```C++
#include <cstdlib>
#include <map>
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
using namespace std;

typedef map<unsigned, string> MyMap;
MyMap my_map;

struct get_second: public std::unary_function<MyMap::value_type, string>
{
	string operator()(const MyMap::value_type &value) const
	{
		return value.second;
	}
};

int main()
{
	my_map[1] = "one";
	my_map[2] = "two";
	my_map[3] = "three";
	my_map[4] = "four";
	my_map[5] = "five";

	// get a vector of values
	vector<string> my_vals;
	transform(my_map.begin(), my_map.end(), back_inserter(my_vals), get_second());

	// dump the list
	copy(my_vals.begin(), my_vals.end(), ostream_iterator<string>(cout, "\n"));
}
// g++ --std=c++11 test.cpp
```

EDIT:

If you have a compiler that supports C++0x [lambdas](http://en.wikipedia.org/wiki/C%2B%2B0x#Lambda_functions_and_expressions), you can eliminate the functor entirely. This is very useful for making code more readable and, arguable, easier to maintain since you don't end up with dozens of little one-off functors floating around in your codebase. Here's how you would change the code above to use a lambda:

```
transform(my_map.begin(), my_map.end(), back_inserter(my_vals), [](const MyMap::value_type& val){return val.second;} );
```



## stackoverflow [Copy map values to vector in STL](https://stackoverflow.com/questions/771453/copy-map-values-to-vector-in-stl)



[A](https://stackoverflow.com/a/771482)

You could probably use [`std::transform`](http://en.cppreference.com/w/cpp/algorithm/transform) for that purpose. I would maybe prefer Neils version though, depending on what is more readable.

------

Example by [xtofl](https://stackoverflow.com/users/6610/xtofl) (see comments):

```C++
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
/**
 * @brief functor
 *
 * @tparam tPair
 */
template<typename tPair>
struct second_t
{
	typename tPair::second_type operator()(const tPair &p) const
	{
		std::cout << __FILE__ << __PRETTY_FUNCTION__ << std::endl;
		return p.second;
	}
};
/**
 * @brief 返回一个functor object
 *
 * @tparam tMap
 * @param m
 * @return
 */
template<typename tMap>
second_t<typename tMap::value_type> second(const tMap &m)
{
	std::cout << __FILE__ << __PRETTY_FUNCTION__ << std::endl;
	return second_t<typename tMap::value_type>();
}

int main()
{
	std::map<int, bool> m;
	m[0] = true;
	m[1] = false;
	//...
	std::vector<bool> v;
	std::transform(m.begin(), m.end(), std::back_inserter(v), second(m));
	std::transform(m.begin(), m.end(), std::ostream_iterator<bool>(std::cout, ";"), second(m));
}
// g++ --std=c++11 test.cpp
```

> NOTE: 
>
> 上述程序所展示的是:" `std::transform` + functor object"的用法，`second_t`是functor，`second`是object generator/factor method，它构建 functor object，从下面的输出可以看出:
>
> 1、`second`会被调用一次，用于构建functor object
>
> 2、functor object会被调用多次，它的调用次数由map 的长度决定
>
> 上述程序的输出如下:
>
> ```C++
> test.cppsecond_t<typename tMap::value_type> second(const tMap&) [with tMap = std::map<int, bool>; typename tMap::value_type = std::pair<const int, bool>]
> test.cpptypename tPair::second_type second_t<tPair>::operator()(const tPair&) const [with tPair = std::pair<const int, bool>; typename tPair::second_type = bool]
> test.cpptypename tPair::second_type second_t<tPair>::operator()(const tPair&) const [with tPair = std::pair<const int, bool>; typename tPair::second_type = bool]
> test.cppsecond_t<typename tMap::value_type> second(const tMap&) [with tMap = std::map<int, bool>; typename tMap::value_type = std::pair<const int, bool>]
> test.cpptypename tPair::second_type second_t<tPair>::operator()(const tPair&) const [with tPair = std::pair<const int, bool>; typename tPair::second_type = bool]
> 1;test.cpptypename tPair::second_type second_t<tPair>::operator()(const tPair&) const [with tPair = std::pair<const int, bool>; typename tPair::second_type = bool]
> ```
>
> 下面是不使用object  generator的版本
>
> ```C++
> #include <map>
> #include <vector>
> #include <algorithm>
> #include <iostream>
> #include <iterator>
> /**
>  * @brief functor
>  *
>  * @tparam tPair
>  */
> template<typename tPair>
> struct second_t
> {
> 	typename tPair::second_type operator()(const tPair &p) const
> 	{
> 		std::cout << __FILE__ << __PRETTY_FUNCTION__ << std::endl;
> 		return p.second;
> 	}
> };
> /**
>  * @brief 返回一个functor object
>  *
>  * @tparam tMap
>  * @param m
>  * @return
>  */
> template<typename tMap>
> second_t<typename tMap::value_type> second(const tMap &m)
> {
> 	std::cout << __FILE__ << __PRETTY_FUNCTION__ << std::endl;
> 	return second_t<typename tMap::value_type>();
> }
> 
> int main()
> {
> 	std::map<int, bool> m;
> 	m[0] = true;
> 	m[1] = false;
> 	//...
> 	std::vector<bool> v;
> 	std::transform(m.begin(), m.end(), std::back_inserter(v), second(m));
> 	std::transform(m.begin(), m.end(), std::ostream_iterator<bool>(std::cout, ";"), second_t<std::map<int, bool>::value_type>());
> }
> // g++ --std=c++11 test.cpp
> ```
>
> 



Very generic, remember to give him credit if you find it useful.



## stackoverflow [How to retrieve all keys (or values) from a std::map and put them into a vector?](https://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap-and-put-them-into-a-vector)

