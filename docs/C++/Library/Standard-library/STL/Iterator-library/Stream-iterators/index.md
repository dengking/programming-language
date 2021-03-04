# Stream iterators

Stream iterator将stream和iterator联系起来，通过iterator，进而连接了algorithm，因此通过stream iterator，可以将algorithm应用于stream。

关于stream，参见`C++\Library\Standard-library\Input-output-library`。

## geeksforgeeks [`std::istream_iterator` and `std::ostream_iterator` in C++ STL](https://www.geeksforgeeks.org/stdistream_iterator-stdostream_iterator-c-stl/)

> NOTE: 这篇文章论述得非常好

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

### Read from std in and print

源自: https://www.geeksforgeeks.org/stdistream_iterator-stdostream_iterator-c-stl/

```C++
// Cpp program to illustrate
// Read a bunch of integers from the input stream
// and print them to output stream

#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;
int main()
{

    // Get input stream and end of stream iterators
    istream_iterator<int> cin_it(cin);
    istream_iterator<int> eos;

    // Get output stream iterators
    ostream_iterator<int> cout_it(cout, " ");

    // We have both input and output iterators, now we can treat them
    // as containers. Using copy function we transfer data from one
    // container to another.
    // Copy elements from input to output using copy function
    copy(cin_it, eos, cout_it);

    return 0;
}
// g++ test.cpp

```

### Read from file and sort

源自: https://www.geeksforgeeks.org/stdistream_iterator-stdostream_iterator-c-stl/

```c++
// Cpp program to illustrate
// Read a bunch of strings from a file
// sort them lexicographically and print them to output stream

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace std;
int main()
{

	// Define a vector to store the strings received from input
	vector < string > strings_v;

	// Define the filestream object used to read data from file
	ifstream fin("input_file.txt");

	// Get input stream and end of stream iterators
	istream_iterator < string > fin_it(fin);
	istream_iterator < string > eos;

	// Get output stream iterators
	ostream_iterator < string > cout_it(cout, " ");

	// Copy elements from input to vector using copy function
	copy(fin_it, eos, back_inserter(strings_v));

	// Sort the vector
	sort(strings_v.begin(), strings_v.end());

	// Copy elements from vector to output
	copy(strings_v.begin(), strings_v.end(), cout_it);

	return 0;
}
// g++ test.cpp

```



> ```
> Contents of File "input_file.txt": quick brown fox jumps over the lazy dog
> Output: brown dog fox jumps lazy over quick the 
> ```



### Read from std in and sort

源自: https://www.geeksforgeeks.org/stdistream_iterator-stdostream_iterator-c-stl/

```C++
// Cpp program to illustrate

// Read a bunch of integers from the stream
// print the sorted order of even integers only

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;
int main()
{

	// Define a vector to store the even integers received from input
	vector<int> vi;

	// Get input stream and end of stream iterators
	istream_iterator<int> cin_it(cin);
	istream_iterator<int> eos;

	// Get output stream iterators
	ostream_iterator<int> cout_it(cout, " ");

	// Copy even integer elements from input to vector using for_each function
	for_each(cin_it, eos, [&](int a) 
	{
		if (a % 2 == 0)
		{
			// if a is even push it to vector
			vi.push_back(a);
		}
	});

	// Sort the vector
	sort(vi.begin(), vi.end());

	// Copy elements from vector to output
	copy(vi.begin(), vi.end(), cout_it);

	return 0;
}
// g++ --std=c++11 test.cpp

```

使用下面的方式进行测试:

```
2
3
4
5
6
8
ctr+D // 表示EOF
```

