# Short logger



## stackoverflow [A better LOG() macro using template metaprogramming](https://stackoverflow.com/questions/19415845/a-better-log-macro-using-template-metaprogramming)

A typical LOG() macro-based logging solution may look something like this:

```cpp
#define LOG(msg) \
    std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl 
```

This allows programmers to create data rich messages using convenient and type-safe streaming operators:

```cpp
string file = "blah.txt";
int error = 123;
...
LOG("Read failed: " << file << " (" << error << ")");

// Outputs:
// test.cpp(5): Read failed: blah.txt (123)
```

The problem is that this causes the compiler to inline multiple `ostream::operator<<` calls. This increases the generated code and therefore function size, which I suspect may hurt **instruction cache performance** and hinder(阻碍) the compiler's ability to optimize the code.

### SOLUTION #2: variadic template function

Here's a "simple" alternative that replaces the inlined code with a call to a **variadic template function**:

`***********SOLUTION #2: VARIADIC TEMPLATE FUNCTION***********`

```c++
#include <iostream>
#include <sstream>
#define LOG(...) LogWrapper(__FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief 需要添加forward declaration，否则会编译报错
 *
 */

void Log_Recursive(const char *file, int line, std::ostringstream &msg);

template<typename ... Args>
void LogWrapper(const char *file, int line, const Args &... args);

// Log_Recursive wrapper that creates the ostringstream
template<typename ... Args>
void LogWrapper(const char *file, int line, const Args &... args)
{
	std::ostringstream msg;
	Log_Recursive(file, line, msg, args...);
}

// "Recursive" variadic function
template<typename T, typename ... Args>
void Log_Recursive(const char *file, int line, std::ostringstream &msg, T value, const Args &... args)
{
	msg << value;
	Log_Recursive(file, line, msg, args...);
}

// Terminator
void Log_Recursive(const char *file, int line, std::ostringstream &msg)
{
	std::cout << file << "(" << line << "): " << msg.str() << std::endl;
}

```

The compiler automatically generates new instantiations of the template function as needed depending on the number, kind, and order of message arguments.

The benefit is there are fewer instructions at each call site. The downside is the user must pass the message parts as function parameters instead of combining them using streaming operators:

```cpp
LOG("Read failed: ", file, " (", error, ")");
```

### SOLUTION #3: EXPRESSION TEMPLATES

At @DyP's suggestion I created an alternative solution that uses **expression templates**:

```cpp
#define LOG(msg) Log(__FILE__, __LINE__, Part<bool, bool>() << msg)

template<typename T> struct PartTrait { typedef T Type; };

// Workaround GCC 4.7.2 not recognizing noinline attribute
#ifndef NOINLINE_ATTRIBUTE
  #ifdef __ICC
    #define NOINLINE_ATTRIBUTE __attribute__(( noinline ))
  #else
    #define NOINLINE_ATTRIBUTE
  #endif // __ICC
#endif // NOINLINE_ATTRIBUTE

// Mark as noinline since we want to minimize the log-related instructions
// at the call sites
template<typename T>
void Log(const char* file, int line, const T& msg) NOINLINE_ATTRIBUTE
{
    std::cout << file << ":" << line << ": " << msg << std::endl;
}

template<typename TValue, typename TPreviousPart>
struct Part : public PartTrait<Part<TValue, TPreviousPart>>
{
    Part()
        : value(nullptr), prev(nullptr)
    { }

    Part(const Part<TValue, TPreviousPart>&) = default;
    Part<TValue, TPreviousPart> operator=(
                           const Part<TValue, TPreviousPart>&) = delete;

    Part(const TValue& v, const TPreviousPart& p)
        : value(&v), prev(&p)
    { }

    std::ostream& output(std::ostream& os) const
    {
        if (prev)
            os << *prev;
        if (value)
            os << *value;
        return os;
    }

    const TValue* value;
    const TPreviousPart* prev;
};

// Specialization for stream manipulators (eg endl)

typedef std::ostream& (*PfnManipulator)(std::ostream&);

template<typename TPreviousPart>
struct Part<PfnManipulator, TPreviousPart>
    : public PartTrait<Part<PfnManipulator, TPreviousPart>>
{
    Part()
        : pfn(nullptr), prev(nullptr)
    { }

    Part(const Part<PfnManipulator, TPreviousPart>& that) = default;
    Part<PfnManipulator, TPreviousPart> operator=(const Part<PfnManipulator,
                                                  TPreviousPart>&) = delete;

    Part(PfnManipulator pfn_, const TPreviousPart& p)
    : pfn(pfn_), prev(&p)
    { }

    std::ostream& output(std::ostream& os) const
    {
        if (prev)
            os << *prev;
        if (pfn)
            pfn(os);
        return os;
    }

    PfnManipulator pfn;
    const TPreviousPart* prev;
};

template<typename TPreviousPart, typename T>
typename std::enable_if<
    std::is_base_of<PartTrait<TPreviousPart>, TPreviousPart>::value, 
    Part<T, TPreviousPart> >::type
operator<<(const TPreviousPart& prev, const T& value)
{
    return Part<T, TPreviousPart>(value, prev);
}

template<typename TPreviousPart>
typename std::enable_if<
    std::is_base_of<PartTrait<TPreviousPart>, TPreviousPart>::value,
    Part<PfnManipulator, TPreviousPart> >::type
operator<<(const TPreviousPart& prev, PfnManipulator value)
{
    return Part<PfnManipulator, TPreviousPart>(value, prev);
}

template<typename TPart>
typename std::enable_if<
    std::is_base_of<PartTrait<TPart>, TPart>::value,
    std::ostream&>::type
operator<<(std::ostream& os, const TPart& part)
{
    return part.output(os);
}
```

> NOTE: 
>
> 1、关于上述程序的分析，参见下面的"SOLUTION #3: EXPRESSION TEMPLATES 完整程序"章节

The expression templates solution allows the programmer to use the familiar convenient and type-safe streaming operators:

```cpp
LOG("Read failed: " << file << " " << error);
```

However, when `Part<A, B>` creation is inlined no operator<< calls are made, giving us the benefit of both worlds: convenient and type-safe streaming operators + fewer instructions. ICC13 with -O3 produces the following assembly code for the above:

```cpp
movl      $.L_2__STRING.3, %edi
movl      $13, %esi
xorl      %eax, %eax
lea       72(%rsp), %rdx
lea       8(%rsp), %rcx
movq      %rax, 16(%rsp)
lea       88(%rsp), %r8
movq      $.L_2__STRING.4, 24(%rsp)
lea       24(%rsp), %r9
movq      %rcx, 32(%rsp)
lea       40(%rsp), %r10
movq      %r8, 40(%rsp)
lea       56(%rsp), %r11
movq      %r9, 48(%rsp)
movq      $.L_2__STRING.5, 56(%rsp)
movq      %r10, 64(%rsp)
movq      $nErrorCode.9291.0.16, 72(%rsp)
movq      %r11, 80(%rsp)
call      _Z3LogI4PartIiS0_IA2_cS0_ISsS0_IA14_cS0_IbbEEEEEENSt9enable_ifIXsr3std10is_base_ofI9PartTraitIT_ESA_EE5valueEvE4typeEPKciRKSA_
```

The total is 19 instructions including one function call. It appears each additional argument streamed adds 3 instructions. The compiler creates a different Log() function instantiation depending on the number, kind, and order of message parts, which explains the bizarre function name.

### SOLUTION #4: CATO'S EXPRESSION TEMPLATES

Here is Cato's excellent solution with a tweak to support stream manipulators (eg endl):

```cpp
#define LOG(msg) (Log(__FILE__, __LINE__, LogData<None>() << msg))

// Workaround GCC 4.7.2 not recognizing noinline attribute
#ifndef NOINLINE_ATTRIBUTE
  #ifdef __ICC
    #define NOINLINE_ATTRIBUTE __attribute__(( noinline ))
  #else
    #define NOINLINE_ATTRIBUTE
  #endif // __ICC
#endif // NOINLINE_ATTRIBUTE

template<typename List>
void Log(const char* file, int line, 
         LogData<List>&& data) NOINLINE_ATTRIBUTE
{
    std::cout << file << ":" << line << ": ";
    output(std::cout, std::move(data.list));
    std::cout << std::endl;
}

struct None { };

template<typename List>
struct LogData {
    List list;
};

template<typename Begin, typename Value>
constexpr LogData<std::pair<Begin&&, Value&&>> operator<<(LogData<Begin>&& begin, 
                                                          Value&& value) noexcept
{
    return {{ std::forward<Begin>(begin.list), std::forward<Value>(value) }};
}

template<typename Begin, size_t n>
constexpr LogData<std::pair<Begin&&, const char*>> operator<<(LogData<Begin>&& begin, 
                                                              const char (&value)[n]) noexcept
{
    return {{ std::forward<Begin>(begin.list), value }};
}

typedef std::ostream& (*PfnManipulator)(std::ostream&);

template<typename Begin>
constexpr LogData<std::pair<Begin&&, PfnManipulator>> operator<<(LogData<Begin>&& begin, 
                                                                 PfnManipulator value) noexcept
{
    return {{ std::forward<Begin>(begin.list), value }};
}

template <typename Begin, typename Last>
void output(std::ostream& os, std::pair<Begin, Last>&& data)
{
    output(os, std::move(data.first));
    os << data.second;
}

inline void output(std::ostream& os, None)
{ }
```

As Cato points out, the benefit over the last solution is that it results in fewer function instantiations since the const char* specialization handles all string literals. It also causes fewer instructions to be generated at the call site:

```cpp
movb  $0, (%rsp)
movl  $.L_2__STRING.4, %ecx
movl  $.L_2__STRING.3, %edi
movl  $20, %esi
lea   212(%rsp), %r9
call  void Log<pair<pair<pair<pair<None, char const*>, string const&>, char const*>, int const&> >(char const*, int, LogData<pair<pair<pair<pair<None, char const*>, string const&>, char const*>, int const&> > const&)
```

**Please let me know if you can think of any way to improve the performance or usability of this solution.**







## SOLUTION #2完整程序

```C++
#include <sstream>
#include <iostream>

#define LOG(...) LogWrapper(__FILE__, __LINE__, __VA_ARGS__)

// Terminator
void Log_Recursive(const char *file, int line, std::ostringstream &msg)
{
	std::cout << file << "(" << line << "): " << msg.str() << std::endl;
}

// "Recursive" variadic function
template<typename T, typename ... Args>
void Log_Recursive(const char *file, int line, std::ostringstream &msg, T value, const Args &... args)
{
	msg << value;
	Log_Recursive(file, line, msg, args...);
}

// Log_Recursive wrapper that creates the ostringstream
template<typename ... Args>
void LogWrapper(const char *file, int line, const Args &... args)
{
	std::ostringstream msg;
	Log_Recursive(file, line, msg, args...);
}

int main()
{
    LOG("hello");
}
// g++ --std=c++11 test.cpp

```

## SOLUTION #3: EXPRESSION TEMPLATES 完整程序

```C++
#include<iostream>
#define LOG(msg) Log(__FILE__, __LINE__, Part<bool, bool>() << msg)

template<typename T>
struct PartTrait
{
	typedef T Type;
};

// Workaround GCC 4.7.2 not recognizing noinline attribute
#ifndef NOINLINE_ATTRIBUTE
#ifdef __ICC
    #define NOINLINE_ATTRIBUTE __attribute__(( noinline ))
  #else
#define NOINLINE_ATTRIBUTE
#endif // __ICC
#endif // NOINLINE_ATTRIBUTE

// Mark as noinline since we want to minimize the log-related instructions
// at the call sites
template<typename T>
void Log(const char *file, int line, const T &msg) NOINLINE_ATTRIBUTE
{
	std::cout << file << ":" << line << ": " << msg << std::endl;
}

template<typename TValue, typename TPreviousPart>
struct Part: public PartTrait<Part<TValue, TPreviousPart>>
{
	/**
	 * @brief 不包含prev，即head node of linked list
	 *
	 */
	Part() :
					value(nullptr), prev(nullptr)
	{
	}

	Part(const Part<TValue, TPreviousPart>&) = default;
	Part<TValue, TPreviousPart> operator=(const Part<TValue, TPreviousPart>&) = delete;
	/**
	 * @brief 包含prev
	 *
	 * @param v
	 * @param p
	 */
	Part(const TValue &v, const TPreviousPart &p) :
					value(&v), prev(&p)
	{
	}

	std::ostream& output(std::ostream &os) const
	{
		if (prev)
			os << *prev;
		if (value)
			os << *value;
		return os;
	}

	const TValue *value;
	const TPreviousPart *prev;
};

// Specialization for stream manipulators (eg endl)

typedef std::ostream& (*PfnManipulator)(std::ostream&);

template<typename TPreviousPart>
struct Part<PfnManipulator, TPreviousPart> : public PartTrait<Part<PfnManipulator, TPreviousPart>>
{
	Part() :
					pfn(nullptr), prev(nullptr)
	{
	}

	Part(const Part<PfnManipulator, TPreviousPart> &that) = default;
	Part<PfnManipulator, TPreviousPart> operator=(const Part<PfnManipulator, TPreviousPart>&) = delete;

	Part(PfnManipulator pfn_, const TPreviousPart &p) :
					pfn(pfn_), prev(&p)
	{
	}

	std::ostream& output(std::ostream &os) const
	{
		if (prev)
			os << *prev;
		if (pfn)
			pfn(os);
		return os;
	}

	PfnManipulator pfn;
	const TPreviousPart *prev;
};

template<typename TPreviousPart, typename T>
typename std::enable_if<std::is_base_of<PartTrait<TPreviousPart>, TPreviousPart>::value, Part<T, TPreviousPart> >::type operator<<(const TPreviousPart &prev, const T &value)
{
	return Part<T, TPreviousPart>(value, prev);
}

template<typename TPreviousPart>
typename std::enable_if<std::is_base_of<PartTrait<TPreviousPart>, TPreviousPart>::value, Part<PfnManipulator, TPreviousPart> >::type operator<<(const TPreviousPart &prev, PfnManipulator value)
{
	return Part<PfnManipulator, TPreviousPart>(value, prev);
}

template<typename TPart>
typename std::enable_if<std::is_base_of<PartTrait<TPart>, TPart>::value, std::ostream&>::type operator<<(std::ostream &os, const TPart &part)
{
	return part.output(os);
}

int main()
{
	LOG("hello"<<","<<"world");
	LOG("hello"<<","<<"world");
}

// g++ --std=c++11 test.cpp

```



1、composition chain

```C++
#define LOG(msg) Log(__FILE__, __LINE__, Part<bool, bool>() << msg)
```

会被替换为:

```C++
Log("test.cpp", 129, Part<bool, bool>() << "Read failed: " << file << " " << error)
```

形成了一个chain、list。

第一个node的prev 是空的，这就相当于最后一个node的next为空。

2、recursion

```C++
    std::ostream& output(std::ostream& os) const
    {
        if (prev)
            os << *prev;
        if (value)
            os << *value;
        return os;
    }
```

通过`prev` pointer，能够从tail逐步找到head。



