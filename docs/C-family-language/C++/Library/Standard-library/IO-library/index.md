# Input/output library

C的IO library是基于stream的，可以说它将stream的概念发扬光大了，C++的I/O library继承了C的IO library，以OOP + TMP的方式对其进行了wrap。

> NOTE: 关于wrap，参见`C-and-C++\Wrapper`

在APUE的《5 Standard I/O Library》中对stream-based I/O进行了非常详细的分析，所以首先阅读APUE的《5 Standard I/O Library》能够帮助理解C++的io library。下面是一些关键性地思想：

APUE的《5.2  Streams and FILE Objects》: 描述了stream的概念。

> When we open or create a file with the standard I/O library, we say that we have associated a stream with the file.

APUE的《5.4  Buffering》

> The goal of the buffering provided by the standard I/O library is to use the minimum number of `read` and `write` calls. (Recall Figure 3.6, which showed the amount of CPU time required to perform I/O using various buffer sizes.) Also, this library tries to do its buffering automatically for each I/O stream, obviating the need for the application to worry about it.

stream-based IO往往是采用buffering策略，c++ IO library也是如此，buffer相关的内容将在`Buffer.md`中进行总结。

## cppreference [Input/output library](Input/output library)



### Stream-based I/O

The stream-based input/output library is organized around abstract input/output devices. These abstract devices allow the same code to handle input/output to **files**, **memory streams**, or **custom adaptor devices** that perform arbitrary operations (e.g. compression) on the fly.

> NOTE: abstraction的威力

Most of the classes are templated, so they can be adapted to any **basic character type**. Separate typedefs are provided for the most common basic character types (`char` and `wchar_t`). 

> NOTE: 上面强调“Most of the classes are templated, so they can be adapted to any **basic character type**”，那这是否说明C++的Stream-based I/O library只能够是character stream呢？这个问题需要结合character stream和byte stream之间的差异、c++ I/O的实现来谈。
>
> 在工程[Linux-OS](https://dengking.github.io/Linux-OS/)的`Programming\IO\IO-流派\Stream`章节中对“character stream和byte stream之间的差异”进行了总结：
>
> > byte stream的unit是byte，character stream的unit是character；一个character由一个或者多个byte组成。
>
> 上面所述的**basic character type**其实包含了：
>
> - `char`
> - `wchar_t`
> - `char16_t`
> - `char32_t`
>
> 显然，`char`其实对应的byte，而其它更宽的char类型对应的是character。显然c++的I/O通过抽象`CharT`，实现了同时支持**byte stream**、**character stream**。
>
> [`std::basic_ios`](https://en.cppreference.com/w/cpp/io/basic_ios)的声明如下：
>
> ```c++
> template<
>     class CharT,
>     class Traits = std::char_traits<CharT>
> > class basic_ios : public std::ios_base
> ```

#### Class hierarchy

The classes are organized into the following hierarchy:

> NOTE: 图略，下面是根据原图，使用table的格式组织的

> NOTE: 两个正交的概念：
>
> - buffer、input、output
> - file、string
>
> 下面表格基于上述两个正交的概念进行绘制，它和原文的结构类似，并且还添加了buffer类列。

|                  | [basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf) | [std::basic_istream](https://en.cppreference.com/w/cpp/io/basic_istream) | [std::basic_ostream](https://en.cppreference.com/w/cpp/io/basic_ostream) | [std::basic_iostream](https://en.cppreference.com/w/cpp/io/basic_iostream) |
| ---------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| **string steam** | [basic_stringbuf](https://en.cppreference.com/w/cpp/io/basic_stringbuf) | [std::basic_istringstream](https://en.cppreference.com/w/cpp/io/basic_istringstream) | [std::basic_ostringstream](https://en.cppreference.com/w/cpp/io/basic_ostringstream) | [std::basic_stringstream](https://en.cppreference.com/w/cpp/io/basic_stringstream) |
| **file stream**  | [basic_filebuf](https://en.cppreference.com/w/cpp/io/basic_filebuf) | [std::basic_ifstream](https://en.cppreference.com/w/cpp/io/basic_ifstream) | [std::basic_ofstream](https://en.cppreference.com/w/cpp/io/basic_ofstream) | [std::basic_fstream](https://en.cppreference.com/w/cpp/io/basic_fstream) |



#### Abstraction

> NOTE: "abstraction"的含义是"基类"、"抽象类"。

|                                                              |                                                              | 注释                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [ios_base](https://en.cppreference.com/w/cpp/io/ios_base)    | manages formatting flags and input/output exceptions (class) |                                                              |
| [basic_ios](https://en.cppreference.com/w/cpp/io/basic_ios)  | manages an arbitrary stream buffer                           | The class `std::basic_ios` provides facilities for interfacing with objects that have [std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf) interface，显然它为了实现buffering IO的。 |
| [basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf) | abstracts a raw device (class template)                      | 这是底层buffer的实现，其它类型的stream的实现都依赖于它，比如<br>- File I/O implementation的[basic_filebuf](https://en.cppreference.com/w/cpp/io/basic_filebuf) <br>- String I/O implementation的[basic_stringbuf](https://en.cppreference.com/w/cpp/io/basic_stringbuf) <br>这些内容将在`Buffer.md`中进行总结 |
| [basic_ostream](https://en.cppreference.com/w/cpp/io/basic_ostream) | wraps a given abstract device ([std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf)) and provides high-level output interface |                                                              |
| [basic_istream](https://en.cppreference.com/w/cpp/io/basic_istream) | wraps a given abstract device ([std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf)) and provides high-level output interface |                                                              |
| [basic_iostream](https://en.cppreference.com/w/cpp/io/basic_iostream) | wraps a given abstract device ([std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf)) and provides high-level input/output interface |                                                              |



#### File I/O implementation

> NOTE: 支持file IO，在前面的"Class hierarchy"章节中，已经对它的implementation class进行了介绍。

Defined in header `<fstream>`

#### String I/O implementation

> NOTE: 支持string IO，在前面的"Class hierarchy"章节中，已经对它的implementation class进行了介绍。

Defined in header `<sstream>`

#### Typedefs

> NOTE: 其实就是一堆已经提供好的typedef

#### Predefined standard stream objects

> NOTE: 从下面可以看出在本文开头所总结的观点: 
>
> > C++的I/O library继承了C的IO library，以OOP + TMP的方式对其进行了wrap

| C++ stream object                                           | standard C stream                                            |
| ----------------------------------------------------------- | ------------------------------------------------------------ |
| [cin <br>wcin](https://en.cppreference.com/w/cpp/io/cin)    | standard C input stream [stdin](https://en.cppreference.com/w/cpp/io/c) |
| [cout <br>wcout](https://en.cppreference.com/w/cpp/io/cout) | standard C output stream [stdout](https://en.cppreference.com/w/cpp/io/c) |
| [cerr <br>wcerr](https://en.cppreference.com/w/cpp/io/cerr) | standard C error stream [stderr](https://en.cppreference.com/w/cpp/io/c), unbuffered |
| [clog <br>wclog](https://en.cppreference.com/w/cpp/io/clog) | standard C error stream [stderr](https://en.cppreference.com/w/cpp/io/c) |



#### [I/O Manipulators](https://en.cppreference.com/w/cpp/io/manip)



### [C-style I/O](https://en.cppreference.com/w/cpp/io/c)

> NOTE: C-style IO也是基于stream的。



## C++ IO library and STL

一般我们不将C++ IO library纳入STL的中，但是C++ IO library的设计理念和STL是一脉相承的，并且由于C++ IO library是stream-based的，而stream又非常类似于container，所以C++ std library的设计者已经充分考虑了两者之间的关联，已经实现了让用户按照STL的模式来处理IO stream，连接两者的桥梁是: 

1) `std::istream_iterator`

2) `std::ostream_iterator`

它们在`C++\Library\Standard-library\STL\Iterator-library\Stream-iterators`中进行了详细介绍。

关于本节总结的思想，在geeksforgeeks [std::istream_iterator and std::ostream_iterator in C++ STL](https://www.geeksforgeeks.org/stdistream_iterator-stdostream_iterator-c-stl/)中进行了较好的论述:

> All three components are so designed that they confirm to the principles of **data abstraction**. Thus any object which holds data and *behaves like a container*, is a container. Similarly, any iterator which sweeps through the elements in a container is an **iterator**.
>
> If an iterator can be used to access elements of a data container, then what about streams? In keeping with the design, Streams too are data containers and so C++ provides us with iterators to iterate over the elements present in any stream. These iterators are called **Stream Iterators**. To use these iterators the *iterator* header file must be included.



## Bit stream

看了一下，c++的io library是不支持bit stream的，那在c++中如何来处理bit stream呢？

https://sakhnik.com/2018/03/23/bitstream.html

http://www.drdobbs.com/bitstream-parsing-in-c/184402014



## Buffer stream

今天写了这样的一个程序

```c++
std::pair<void*, size_t> Pack()
{
	MsgHeaderType Header;
	Header.MsgHead.nSenderID = 1;
	Header.MsgHead.nToken = 1;
	Header.DataHead.uResponseNumber = 3;

	CHSInsFundDataPushField F1;
	std::memset(&F1, 0, sizeof(CHSInsFundDataPushField));
	F1.ProductIndex = 1;
	F1.ProductNo = 1;
	std::strcpy(F1.ProductRef, "1");

	CHSInsFundDataPushField F2;
	std::memset(&F2, 0, sizeof(CHSInsFundDataPushField));
	F2.ProductIndex = 2;
	F2.ProductNo = 2;
	std::strcpy(F2.ProductRef, "2");

	CHSInsFundDataPushField F3;
	std::memset(&F3, 0, sizeof(CHSInsFundDataPushField));
	F3.ProductIndex = 3;
	F3.ProductNo = 3;
	std::strcpy(F3.ProductRef, "3");

	std::size_t size = sizeof(MsgHeaderType) + 3 * sizeof(CHSInsFundDataPushField);
	void*Buffer = std::malloc(size);
	std::memcpy(Buffer, &Header, sizeof(MsgHeaderType));
	std::memcpy((char*) Buffer + sizeof(MsgHeaderType), &F1, sizeof(CHSInsFundDataPushField));
	std::memcpy((char*) Buffer + sizeof(MsgHeaderType) + sizeof(CHSInsFundDataPushField), &F2, sizeof(CHSInsFundDataPushField));
	std::memcpy((char*) Buffer + sizeof(MsgHeaderType) + sizeof(CHSInsFundDataPushField) * 2, &F3, sizeof(CHSInsFundDataPushField));
	return std::pair<void*, size_t> { Buffer, size };
}
```

这个程序其实非常简单，即将一些struct object输出到一个buffer中，我想的是在c++中有没有更加灵活的方式来实现它？我的第一想法是stream，即我需要一个buffer stream，它可以实现类似于`stringstream`的`<<`操作，下面是查询到的一些与此相关的内容：

[What exactly is streambuf? How do I use it?](https://stackoverflow.com/questions/8116541/what-exactly-is-streambuf-how-do-i-use-it)

[A dynamic buffer type in C++?](https://stackoverflow.com/questions/1874354/a-dynamic-buffer-type-in-c)

另外与此相关的有：c++ byte array，因为我们一般使用byte array来作为buffer。这让我想到了redis的做法，redis为每个client都添加了一个buffer。