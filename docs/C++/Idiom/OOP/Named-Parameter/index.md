# Named parameter



## More C++ Idioms [Named Parameter](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Named_Parameter)



## isocpp What is the “Named Parameter Idiom”? [¶](https://isocpp.org/wiki/faq/ctors#named-parameter-idiom) [Δ](https://isocpp.org/wiki/faq/ctors#)

It’s a fairly useful way to exploit [method chaining](https://isocpp.org/wiki/faq/references#method-chaining).

The fundamental problem solved by the Named Parameter Idiom is that C++ only supports *positional parameters*. 

> NOTE: 所谓named parameter，其实就是python中的`kwargs`。

```c++
#include <string>

class File;
class OpenFile
{
public:
	OpenFile(const std::string& filename);
	// sets all the default values for each data member
	OpenFile& readonly();  // changes readonly_ to true
	OpenFile& readwrite(); // changes readonly_ to false
	OpenFile& createIfNotExist();
	OpenFile& blockSize(unsigned nbytes);
	// ...
private:
	friend class File;
	std::string filename_;
	bool readonly_;          // defaults to false [for example]
	bool createIfNotExist_;  // defaults to false [for example]
	// ...
	unsigned blockSize_;     // defaults to 4096 [for example]
	// ...
};
inline OpenFile::OpenFile(const std::string& filename)
		: filename_(filename)
				, readonly_(false)
				, createIfNotExist_(false)
				, blockSize_(4096u)
{
}
inline OpenFile& OpenFile::readonly()
{
	readonly_ = true;
	return *this;
}
inline OpenFile& OpenFile::readwrite()
{
	readonly_ = false;
	return *this;
}
inline OpenFile& OpenFile::createIfNotExist()
{
	createIfNotExist_ = true;
	return *this;
}
inline OpenFile& OpenFile::blockSize(unsigned nbytes)
{
	blockSize_ = nbytes;
	return *this;
}

class File
{
public:
	File(const OpenFile& params);
	// ...
};

File::File(const OpenFile& params)
{
	// ...
}

int main()
{
	File f = OpenFile("foo.txt")
			.readonly()
			.createIfNotExist()
			.blockSize(1024);
}
```

编译`g++ tests.cpp`

## wikipedia [Named parameter#With chained method calls](https://en.wikipedia.org/wiki/Named_parameter#With_chained_method_calls)





## TO READ 

https://marcoarena.wordpress.com/2014/12/16/bring-named-parameters-in-modern-cpp/

https://stackoverflow.com/questions/40533193/v8-c-how-to-get-object-key-values-provided-as-arguments

https://www.fluentcpp.com/2018/12/14/named-arguments-cpp/