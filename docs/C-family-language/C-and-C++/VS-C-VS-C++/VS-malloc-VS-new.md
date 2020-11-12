[TOC]

# [Out of memory](https://en.wikipedia.org/wiki/Out_of_memory) 的处理

 the [C standard function](https://en.wikipedia.org/wiki/C_standard_function) for allocating memory, `malloc()`, will return [NULL](https://en.wikipedia.org/wiki/Null_pointer) and a well-behaved application should handle this situation. `malloc`通过返回值是否为NULL来判断是否分配成功

c++中的`new`呢？通过Google `c++ new return null`可以得到如下有文章：

- [Why doesn't new in C++ return NULL on failure](https://stackoverflow.com/questions/26419786/why-doesnt-new-in-c-return-null-on-failure)
- http://www.cplusplus.com/reference/new/bad_alloc/
- [Will new return NULL in any case?](https://stackoverflow.com/questions/550451/will-new-return-null-in-any-case)
- 

以下是demo

```c
void alloc_resource(int len) {
		try {
			/// https://stackoverflow.com/a/2204380
			///  initialise memory with new operator in C++
			array_p_ = new ElementT[len]();
		} catch (std::bad_alloc& ba) {
			std::string msg = "bad_alloc caught: " + ba.what();
			std::cerr << msg << '\n';
			LogError(msg);
			std::abort();
		}
		len_ = len;
	}
```

