# Buffer



## cppreference [std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf)

The class `basic_streambuf` controls input and output to a **character sequence**.

> NOTE: 有两个**character sequence**：
>
> 1) *controlled character sequence*，buffer
>
> 2) *associated character sequence*，source or sink

It includes and provides access to

1) The *controlled character sequence*, also called the *buffer*, which may contain *input sequence* (also called *get area*) for buffering the input operations and/or *output sequence* (also called *put area*) for buffering the output operations.

2) The *associated character sequence*, also called *source* (for input) or *sink* (for output). This may be an entity that is accessed through OS API (file, TCP socket, serial port, other character device), or it may be an object (`std::vector`, array, string literal), that can be interpreted as a character source or sink.

A `basic_streambuf` object may support 

- **input** (in which case the buffer described by the **beginning**, **next**, and **end** pointers is called *get area*), 
- **output** (*put area*), 
- input and output simultaneously. 

In latter case, six pointers are tracked, which may all point to elements of the same character array or two individual arrays.

![std-streambuf.svg](http://upload.cppreference.com/mwiki/images/7/75/std-streambuf.svg)

> NOTE: 
>
> 上面这张图基本上已经形象的描述了`basic_streambuf`的原理了。
>
> 在上一章中，我们已经知道C++ IO library采用了buffering策略，对应的就是**input buffer**、**output buffer**。
>
> 上图展示了两个数据流向：
>
> **input/read**：
>
> 如果input buffer为empty，即无数据可读、即`in_avail()`返回0、即next pointer等于`egptr()`，则调用`underflow()`来从**associated character sequence**中读取数据，数据的流向如下：
>
> associated character sequence ->  `codecvt` -> input buffer 
>
> 此时input buffer为full。然后application可以调用`sgetc()`，即“The next pointer can be dereferenced and read from."
>
> **output/write**：
>
> 如果output buffer为full，即无空间可写、即next pointer等于`egptr()`，则调用`overflow()`来将数据写入到associated character sequence中，数据的流向如下：
>
> output buffer -> `codecvt` -> associated character sequence
>
> 此时output buffer为empty。如何application可以调用`sputc()`，即“The next pointer can be dereferenced and assigned to.”

If the **next pointer** is less than the **end pointer** in the **put area**, a *write position* is available. The next pointer can be dereferenced and assigned to.

If the **next pointer** is less than the **end pointer** in the **get area**, a *read position* is available. The next pointer can be dereferenced and read from.

If the next pointer is greater than the beginning pointer in a get area, a *putback position* is available, and the next pointer may be decremented, dereferenced, and assigned to, in order to put a character back into the get area.

The character representation and encoding in the **controlled sequence** may be different from the character representations in the **associated sequence**, in which case a [std::codecvt](https://en.cppreference.com/w/cpp/locale/codecvt) locale facet is typically used to perform the conversion. Common examples are UTF-8 (or other multibyte) files accessed through [std::wfstream](https://en.cppreference.com/w/cpp/io/basic_fstream) objects: the **controlled sequence** consists of `wchar_t` characters, but the **associated sequence** consists of bytes.

Typical implementation of the `std::basic_streambuf` base class holds only the six `CharT*` pointers and a copy of [std::locale](https://en.cppreference.com/w/cpp/locale/locale) as data members. In addition, implementations may keep cached copies of locale facets, which are invalidated whenever `imbue()` is called. The concrete buffers such as [std::basic_filebuf](https://en.cppreference.com/w/cpp/io/basic_filebuf) or [std::basic_stringbuf](https://en.cppreference.com/w/cpp/io/basic_stringbuf) are derived from `std::basic_streambuf`.



![std-streambuf.svg](http://upload.cppreference.com/mwiki/images/7/75/std-streambuf.svg)

|                                                          | Get area                                                     | Put area                                                     |
| -------------------------------------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| get the pointer to the <br>**beginning** of the area     | [`eback()`](https://en.cppreference.com/w/cpp/io/basic_streambuf/gptr) | [`pbase()`](https://en.cppreference.com/w/cpp/io/basic_streambuf/pptr) |
| get the pointer to the <br>current character in the area | [`gptr()`](https://en.cppreference.com/w/cpp/io/basic_streambuf/gptr) | [`pptr()`](https://en.cppreference.com/w/cpp/io/basic_streambuf/pptr) |
| get the pointer to the <br/>end of the area              | [`egptr()`](https://en.cppreference.com/w/cpp/io/basic_streambuf/gptr) | [`epptr()`](https://en.cppreference.com/w/cpp/io/basic_streambuf/pptr) |
| advances the next pointer                                | [gbump](https://en.cppreference.com/w/cpp/io/basic_streambuf/gbump) | [pbump](https://en.cppreference.com/w/cpp/io/basic_streambuf/pbump) |
| repositions the pointers                                 | [setg](https://en.cppreference.com/w/cpp/io/basic_streambuf/setg) | [setp](https://en.cppreference.com/w/cpp/io/basic_streambuf/setp) |



|      | Get area                                                     | Put area                                                     |
| ---- | ------------------------------------------------------------ | ------------------------------------------------------------ |
|      | [sgetc](https://en.cppreference.com/w/cpp/io/basic_streambuf/sgetc) | [sputc](https://en.cppreference.com/w/cpp/io/basic_streambuf/sputc) |
|      | [sgetn](https://en.cppreference.com/w/cpp/io/basic_streambuf/sgetn) | [sputn](https://en.cppreference.com/w/cpp/io/basic_streambuf/sputn) |
|      | [xsgetn](https://en.cppreference.com/w/cpp/io/basic_streambuf/sgetn) | [xsputn](https://en.cppreference.com/w/cpp/io/basic_streambuf/sputn) |



| Get area                                                     | Put area                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| [underflow](https://en.cppreference.com/w/cpp/io/basic_streambuf/underflow) [virtual] | [overflow](https://en.cppreference.com/w/cpp/io/basic_streambuf/overflow) [virtual] |




## cppreference [std::basic_stringbuf](https://en.cppreference.com/w/cpp/io/basic_stringbuf)

`std::basic_stringbuf` is a [std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf) whose **associated character sequence** is a memory-resident sequence of arbitrary characters, which can be initialized from or made available as an instance of [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string).

Typical implementations of `std::basic_stringbuf` hold an object of type [std::basic_string](https://en.cppreference.com/w/cpp/string/basic_string) or equivalent resizeable sequence container directly as a data member and use it as both the controlled character sequence (the array where the six pointers of [std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf) are pointing to) and as the associated character sequence (the source of characters for all input operations and the target for the output).



## cppreference [std::basic_filebuf](https://en.cppreference.com/w/cpp/io/basic_filebuf)



`std::basic_filebuf` is a [std::basic_streambuf](https://en.cppreference.com/w/cpp/io/basic_streambuf) whose **associated character sequence** is a file. Both the **input sequence** and the **output sequence** are associated with the same file, and a joint file position is maintained for both operations.

The functions `underflow()` and `overflow()`/`sync()` perform the actual I/O between the file and the get and put areas of the buffer. When `CharT` is not char, most implementations store multibyte characters in the file and a [std::codecvt](https://en.cppreference.com/w/cpp/locale/codecvt) facet is used to perform wide/multibyte character conversion.