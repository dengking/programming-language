# Stream iterators

stream iterator将stream和iterator联系起来，通过iterator，进而连接了algorithm，因此通过streamiterator，可以将algorithm应用于stream。

## `ostream_iterator`

把这两种output stream iterator放到一起来进行介绍。

### cppreference [std::ostream_iterator](https://en.cppreference.com/w/cpp/iterator/ostream_iterator)





### cppreference [std::ostreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/ostreambuf_iterator)



### [std::ostream_iterator](https://en.cppreference.com/w/cpp/iterator/ostream_iterator) VS [std::ostreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/ostreambuf_iterator)

在[std::ostream_iterator](https://en.cppreference.com/w/cpp/iterator/ostream_iterator)中有这样的描述：

> When writing characters, [std::ostreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/ostreambuf_iterator) is more efficient, since it avoids the overhead of constructing and destructing the sentry object once per character.

显然[std::ostream_iterator](https://en.cppreference.com/w/cpp/iterator/ostream_iterator)需要sentry object，那sentry object是什么呢？

而[std::ostreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/ostreambuf_iterator)是通过：

> a boolean flag indicating if the the **end of file condition** has been reached





## `istream_iterator`



### cppreference [std::istream_iterator](https://en.cppreference.com/w/cpp/iterator/istream_iterator)



`std::istream_iterator` is a single-pass input iterator that reads successive objects of type `T` from the [std::basic_istream](https://en.cppreference.com/w/cpp/io/basic_istream) object for which it was constructed, by calling the appropriate `operator>>`. 

> NOTE: `std::istream_iterator`的特性：
>
> - single pass
> - input iterator 



The default-constructed `std::istream_iterator` is known as the *end-of-stream* iterator. When a valid `std::istream_iterator` reaches the end of the underlying stream, it becomes equal to the end-of-stream iterator. Dereferencing or incrementing it further invokes undefined behavior.

> NOTE: 在下面的example中，就用到了“The default-constructed `std::istream_iterator` is known as the *end-of-stream* iterator. ”来标志end-of-stream。

#### Notes

When reading characters, `std::istream_iterator` skips **whitespace** by default (unless disabled with [std::noskipws](https://en.cppreference.com/w/cpp/io/manip/skipws) or equivalent), while [std::istreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator) does not. In addition, [std::istreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator) is more efficient, since it avoids the overhead of constructing and destructing the sentry object once per character.



#### Example

```c++
#include <iostream>
#include <sstream>
#include <iterator>
#include <numeric>
#include <algorithm>
 
int main()
{
    std::istringstream str("0.1 0.2 0.3 0.4");
    std::partial_sum(std::istream_iterator<double>(str),
                     std::istream_iterator<double>(),
                     std::ostream_iterator<double>(std::cout, " "));
 
    std::istringstream str2("1 3 5 7 8 9 10");
    std::cout << "\nThe first even number is " <<
        *std::find_if(std::istream_iterator<int>(str2),
                      std::istream_iterator<int>(),
                      [](int i){return i%2 == 0;})
        << ".\n";
    //" 9 10" left in the stream
}
```

> NOTE: `std::istream_iterator<double>()`原来标识end-of-stream。



### cppreference [std::istreambuf_iterator](https://en.cppreference.com/w/cpp/iterator/istreambuf_iterator)





## Example: output container



### Output to std out

取自: stackoverflow [How to use ostream_iterator<> to output result to a newfile?](https://stackoverflow.com/questions/22876893/how-to-use-ostream-iterator-to-output-result-to-a-newfile)

```c++
// ostream_iterator example
#include <iostream>     // std::cout
#include <iterator>     // std::ostream_iterator
#include <vector>       // std::vector
#include <algorithm>    // std::copy

int main()
{
	std::vector<int> myvector;
	for (int i = 1; i < 10; ++i)
	{
		myvector.push_back(i * 10);
	}

	std::ostream_iterator<int> out_it(std::cout, ", ");
	std::copy(myvector.begin(), myvector.end(), out_it);
	return 0;
}
// g++ test.cpp

```

取自: https://www.tenouk.com/cpluscodesnippet/cplusstlostream_iteratorconstructor.html



```c++
// C++ STL ostream_iterator::ostream_iterator, constructor

#include <iterator>
#include <vector>
#include <iostream>

using namespace std;

int main(void)

{

	// C++ STL ostream_iterator for stream cout
	cout << "Inserting & printing elements: " << endl;
	ostream_iterator<int> intOut(cout, "\n");
	// insert data
	*intOut = 12;
	intOut++;
	*intOut = 33;
	intOut++;
	int i;

// vector container
	vector<int> vec;
// push/insert data into vec vector
	for (i = 10; i <= 15; ++i)
	{
		vec.push_back(i);
	}
	cout << "Operation: with and without delimiter..." << endl;

// write elements to standard output stream

	cout << "Elements output without delimiter: ";
	copy(vec.begin(), vec.end(), ostream_iterator<int>(cout));
	cout << endl;

// write elements with delimiter " " to output stream
	cout << "Elements output with delimiter: ";
	copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	return 0;
}
// g++ test.cpp

```



### Output to file

取自: https://stackoverflow.com/a/22876942

````C++
// ostream_iterator example
#include <iostream>     // std::cout
#include <iterator>     // std::ostream_iterator
#include <vector>       // std::vector
#include <algorithm>    // std::copy
#include <fstream>      // std::ofstream

int main()
{
	std::vector<int> myvector;
	for (int i = 1; i < 10; ++i)
	{
		myvector.push_back(i * 10);
	}

	std::ofstream of("myoutput.txt");

	std::ostream_iterator<int> out_it(of, ", ");
	std::copy(myvector.begin(), myvector.end(), out_it);
	return 0;
}
// g++ test.cpp

````

