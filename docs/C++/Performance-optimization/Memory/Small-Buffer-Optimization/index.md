# Small Object Optimization

参考: 

1、riptutorial [C++ Small Object Optimization](https://riptutorial.com/cplusplus/example/31654/small-object-optimization)

2、`C++\Programming-paradigm\Polymorphic-value-type\Type-Erasure\Small-object-buffer-optimization`

3、cppcoreguideline中也有对Small Object Optimization的描述

## riptutorial [C++ Small Object Optimization](https://riptutorial.com/cplusplus/example/31654/small-object-optimization)

Small object optimization is a technique which is used within low level data structures, for instance the `std::string` (Sometimes referred to as Short/Small String Optimization). It's meant to use stack space as a buffer instead of some allocated memory in case the content is small enough to fit within the reserved space.

> NOTE: 
>
> 1、stack  VS heap

By adding extra memory overhead and extra calculations, it tries to prevent an expensive heap allocation. The benefits of this technique are dependent on the usage and can even hurt performance if incorrectly used.

### Example

A very naive way of implementing a string with this optimization would the following:

```C++
#include <cstring>

class string final
{
	constexpr static auto SMALL_BUFFER_SIZE = 16;

	bool _isAllocated { false };                       ///< Remember if we allocated memory
	char *_buffer { nullptr };                         ///< Pointer to the buffer we are using
	char _smallBuffer[SMALL_BUFFER_SIZE] = { '\0' };   ///< Stack space used for SMALL OBJECT OPTIMIZATION

public:
	~string()
	{
		if (_isAllocated)
			delete[] _buffer;
	}

	explicit string(const char *cStyleString)
	{
		auto stringSize = std::strlen(cStyleString);
		_isAllocated = (stringSize > SMALL_BUFFER_SIZE);
		if (_isAllocated)
			_buffer = new char[stringSize];
		else
			_buffer = &_smallBuffer[0];
		std::strcpy(_buffer, &cStyleString[0]);
	}

	string(string &&rhs) :
					_isAllocated(rhs._isAllocated), _buffer(rhs._buffer), _smallBuffer(rhs._smallBuffer) //< Not needed if allocated
	{
		if (_isAllocated)
		{
			// Prevent double deletion of the memory
			rhs._buffer = nullptr;
		}
		else
		{
			// Copy over data
			std::strcpy(_smallBuffer, rhs._smallBuffer);
			_buffer = &_smallBuffer[0];
		}
	}
	// Other methods, including other constructors, copy constructor,
	// assignment operators have been omitted for readability
};

```

> NOTE: 
>
> 1、
>
> ```C++
> string(string &&rhs) :
> 					_isAllocated(rhs._isAllocated), _buffer(rhs._buffer), _smallBuffer(rhs._smallBuffer) //< Not needed if allocated
> ```
>
> `_smallBuffer(rhs._smallBuffer)` 会导致编译报错:
>
> ```C++
> test.cpp:30:60: error: array initializer must be an initializer list or string literal
>                                      _isAllocated(rhs._isAllocated), _buffer(rhs._buffer), _smallBuffer(rhs._smallBuffer) //< Not needed if allocated
>                                                                                            ^
> 
> ```
>
> 2、上述是典型的遵循 CppCoreGuidelines [C.64: A move operation should move and leave its source in a valid state](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#c64-a-move-operation-should-move-and-leave-its-source-in-a-valid-state)

As you can see in the code above, some extra complexity has been added in order to prevent some `new` and `delete` operations. On top of this, the class has a larger memory footprint which might not be used except in a couple of cases.



Often it is tried to encode the `bool` value `_isAllocated`, within the pointer `_buffer` with [bit manipulation](https://riptutorial.com/cplusplus/topic/3016/bit-manipulation) to reduce the size of a single instance (intel 64 bit: Could reduce size by 8 byte). An optimization which is only possible when its known what the alignment rules of the platform is.



### 完整测试程序

```C++
#include <cstring>

class string final
{
	constexpr static auto SMALL_BUFFER_SIZE = 16;

	bool _isAllocated { false };                       ///< Remember if we allocated memory
	char *_buffer { nullptr };                         ///< Pointer to the buffer we are using
	char _smallBuffer[SMALL_BUFFER_SIZE] = { '\0' };   ///< Stack space used for SMALL OBJECT OPTIMIZATION

public:
	~string()
	{
		if (_isAllocated)
			delete[] _buffer;
	}

	explicit string(const char *cStyleString)
	{
		auto stringSize = std::strlen(cStyleString);
		_isAllocated = (stringSize > SMALL_BUFFER_SIZE);
		if (_isAllocated)
			_buffer = new char[stringSize];
		else
			_buffer = &_smallBuffer[0];
		std::strcpy(_buffer, &cStyleString[0]);
	}

	string(string &&rhs) :
					_isAllocated(rhs._isAllocated), _buffer(rhs._buffer)   //, _smallBuffer(rhs._smallBuffer) //< Not needed if allocated
	{
		if (_isAllocated)
		{
			// Prevent double deletion of the memory
			rhs._buffer = nullptr;
		}
		else
		{
			// Copy over data
			std::strcpy(_smallBuffer, rhs._smallBuffer);
			_buffer = &_smallBuffer[0];
		}
	}
	// Other methods, including other constructors, copy constructor,
	// assignment operators have been omitted for readability
};

int main()
{
	string s { "hello" };
}

// g++ --std=c++11 test.cpp

```

### When to use?[#](https://riptutorial.com/cplusplus/example/31654/small-object-optimization#undefined)

As this optimization adds a lot of complexity, it is not recommended to use this optimization on every single class. It will often be encountered in commonly used, low-level data structures. In common C++11 `standard library` implementations one can find usages in [`std::basic_string<>`](https://riptutorial.com/cplusplus/topic/488/std--string) and [`std::function<>`](https://riptutorial.com/cplusplus/topic/2294/std--function--to-wrap-any-element-that-is-callable).
