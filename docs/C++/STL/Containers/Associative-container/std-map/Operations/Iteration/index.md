# Operation: Iteration

参考文章：

1) stackoverflow [C++ Loop through Map](https://stackoverflow.com/questions/26281979/c-loop-through-map)

2) thispointer [How to Iterate over a map in C++](https://thispointer.com/how-to-iterate-over-a-map-in-c/)

## 使用普通的iterator

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

## Iterating over the map using C++11 range based for loop

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

### 使用type inference

```c++
for (auto const& x : symbolTable)
{
    std::cout << x.first  // string (key)
              << ':' 
              << x.second // string's value 
              << std::endl ;
}
```

#### 使用C++17 [Structured binding](https://en.cppreference.com/w/cpp/language/structured_binding)

```cpp
for( auto const& [key, val] : symbolTable )
{
    std::cout << key         // string (key)
              << ':'  
              << val        // string's value
              << std::endl ;
}
```



```
for( auto && [key, val] : symbolTable )
{
    std::cout << key         // string (key)
              << ':'  
              << val        // string's value
              << std::endl ;
}
```



## Iterating over the map using `std::for_each` and lambda function

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

