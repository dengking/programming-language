# Operations

本文描述对map的各种操作。

## Operation: Iteration

参考文章：

1) stackoverflow [C++ Loop through Map](https://stackoverflow.com/questions/26281979/c-loop-through-map)

2) thispointer [How to Iterate over a map in C++](https://thispointer.com/how-to-iterate-over-a-map-in-c/)

### 使用普通的iterator

关于map的iterator，参见

使用`for`

```c++
map<string, int>::iterator it;

for ( it = symbolTable.begin(); it != symbolTable.end(); it++ )
{
    std::cout << it->first  // string (key)
              << ':'
              << it->second   // string's value 
              << std::endl ;
}
```

使用`while`

```c++
#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>
 
int main() {
 
	std::map<std::string, int> mapOfWordCount;
	// Insert Element in map
	mapOfWordCount.insert(std::pair<std::string, int>("first", 1));
	mapOfWordCount.insert(std::pair<std::string, int>("second", 2));
	mapOfWordCount.insert(std::pair<std::string, int>("third", 3));
	mapOfWordCount.insert(std::pair<std::string, int>("third", 4));
	mapOfWordCount.insert(std::pair<std::string, int>("third", 5));
 
	// Create a map iterator and point to beginning of map
	std::map<std::string, int>::iterator it = mapOfWordCount.begin();
 
	// Iterate over the map using Iterator till end.
	while (it != mapOfWordCount.end())
	{
		// Accessing KEY from element pointed by it.
		std::string word = it->first;
 
		// Accessing VALUE from element pointed by it.
		int count = it->second;
 
		std::cout << word << " :: " << count << std::endl;
 
		// Increment the Iterator to point to next entry
		it++;
	}
	return 0;
}
```

### Iterating over the map using C++11 range based for loop

参见[Range-based for loop](https://en.cppreference.com/w/cpp/language/range-for)

```c++
#include <map>
#include <string>
#include <iterator>
#include <algorithm>
 
int main() {
 
	std::map<std::string, int> mapOfWordCount;
	// Insert Element in map
	mapOfWordCount.insert(std::pair<std::string, int>("first", 1));
	mapOfWordCount.insert(std::pair<std::string, int>("second", 2));
	mapOfWordCount.insert(std::pair<std::string, int>("third", 3));
	mapOfWordCount.insert(std::pair<std::string, int>("third", 4));
	mapOfWordCount.insert(std::pair<std::string, int>("third", 5));
 
	// Create a map iterator and point to beginning of map
	std::map<std::string, int>::iterator it = mapOfWordCount.begin();
 
	// Iterate over the map using c++11 range based for loop
	for (std::pair<std::string, int> element : mapOfWordCount) {
		// Accessing KEY from element
		std::string word = element.first;
		// Accessing VALUE from element.
		int count = element.second;
		std::cout << word << " :: " << count << std::endl;
	}
 
	return 0;
}
```

因为map中的元素的类型是`std::pair`。

#### 使用type inference

```c++
for (auto const& x : symbolTable)
{
    std::cout << x.first  // string (key)
              << ':' 
              << x.second // string's value 
              << std::endl ;
}
```

使用[Structured binding](https://en.cppreference.com/w/cpp/language/structured_binding)

```cpp
for( auto const& [key, val] : symbolTable )
{
    std::cout << key         // string (key)
              << ':'  
              << val        // string's value
              << std::endl ;
}
```



### Iterating over the map using `std::for_each` and lambda function

```c++
#include <iostream>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>
 
int main() {
 
	std::map<std::string, int> mapOfWordCount;
	// Insert Element in map
	mapOfWordCount.insert(std::pair<std::string, int>("first", 1));
	mapOfWordCount.insert(std::pair<std::string, int>("second", 2));
	mapOfWordCount.insert(std::pair<std::string, int>("third", 3));
	mapOfWordCount.insert(std::pair<std::string, int>("third", 4));
	mapOfWordCount.insert(std::pair<std::string, int>("third", 5));
 
	// Create a map iterator and point to beginning of map
	std::map<std::string, int>::iterator it = mapOfWordCount.begin();
 
	// Iterate over a map using std::for_each and Lambda function
		std::for_each(mapOfWordCount.begin(), mapOfWordCount.end(),
				[](std::pair<std::string, int> element){
					// Accessing KEY from element
					std::string word = element.first;
					// Accessing VALUE from element.
					int count = element.second;
					std::cout<<word<<" :: "<<count<<std::endl;
		});
 
	return 0;
}
```

这种写法有些类似于函数式编程。



## Operation: 删除map中符合条件的元素

https://www.cnblogs.com/dabaopku/p/3912662.html

https://bbs.csdn.net/topics/391899953

https://blog.csdn.net/tieshuxianrezhang/article/details/79221646

https://stackoverflow.com/questions/800955/remove-if-equivalent-for-stdmap

https://en.cppreference.com/w/cpp/algorithm/remove

http://www.cplusplus.com/forum/general/131917/



## Operation: `values` like python dict



[How to copy all Values from a Map to a Vector in C++](https://thispointer.com/how-to-copy-all-values-from-a-map-to-a-vector-in-c/)

[Getting a list of values from a map](https://stackoverflow.com/questions/4195611/getting-a-list-of-values-from-a-map)



[How to retrieve all keys (or values) from a std::map and put them into a vector?](https://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap-and-put-them-into-a-vector)